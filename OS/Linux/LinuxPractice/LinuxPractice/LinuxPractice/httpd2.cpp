/*
    httpd2.c -- standalone HTTP 1.0 server

    Copyright (c) 2004,2005,2017 Minero Aoki

    This program is free software.
    Redistribution and use in source and binary forms,
    with or without modification, are permitted.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <syslog.h>
#define _GNU_SOURCE
#include <getopt.h>

/****** Constants ********************************************************/

#define SERVER_NAME "LittleHTTP"
#define SERVER_VERSION "1.0"
#define HTTP_MINOR_VERSION 0
#define BLOCK_BUF_SIZE 1024
#define LINE_BUF_SIZE 4096
#define MAX_REQUEST_BODY_LENGTH (1024 * 1024)
#define MAX_BACKLOG 5
#define DEFAULT_PORT "80"

/****** Data Type Definitions ********************************************/

struct HTTPHeaderField {
    char* name;
    char* value;
    struct HTTPHeaderField* next;
};

struct HTTPRequest {
    int protocol_minor_version;
    char* method;
    char* path;
    struct HTTPHeaderField* header;
    char* body;
    long length;
};

struct FileInfo {
    char* path;
    long size;
    int ok;
};

/****** Function Prototypes **********************************************/

static void setup_environment(char* root, char* user, char* group);
typedef void (*sighandler_t)(int);
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void detach_children(void);
static void signal_exit(int sig);
static void noop_handler(int sig);
static void become_daemon(void);
static int listen_socket(char* port);
static void server_main(int server, char* docroot);
static void service(FILE* in, FILE* out, char* docroot);
static struct HTTPRequest* read_request(FILE* in);
static void read_request_line(struct HTTPRequest* req, FILE* in);
static struct HTTPHeaderField* read_header_field(FILE* in);
static void upcase(char* str);
static void free_request(struct HTTPRequest* req);
static long content_length(struct HTTPRequest* req);
static char* lookup_header_field_value(struct HTTPRequest* req, char* name);
static void respond_to(struct HTTPRequest* req, FILE* out, char* docroot);
static void do_file_response(struct HTTPRequest* req, FILE* out, char* docroot);
static void method_not_allowed(struct HTTPRequest* req, FILE* out);
static void not_implemented(struct HTTPRequest* req, FILE* out);
static void not_found(struct HTTPRequest* req, FILE* out);
static void output_common_header_fields(struct HTTPRequest* req, FILE* out, char* status);
static struct FileInfo* get_fileinfo(char* docroot, char* path);
static char* build_fspath(char* docroot, char* path);
static void free_fileinfo(struct FileInfo* info);
static char* guess_content_type(struct FileInfo* info);
static void* xmalloc(size_t sz);
static void log_exit(const char* fmt, ...);

/****** Functions ********************************************************/

#define USAGE "Usage: %s [--port=n] [--chroot --user=u --group=g] [--debug] <docroot>\n"

static int debug_mode = 0;

// 짧은 옵션이 아니라, 긴 옵션을 사용한다.
static struct option longopts[] = {
    // >> 프로그램을 디버깅 모드로 실행하는 옵션
    // 
    // >> 디버그 모드에서는 데몬이 되지 않고, 표준 입출력을 단말에 연결한 상태로 실행된다.
    // 이를 통해 에러 메세지를 표준 에러 출력에 출력할 수 있다.
    // 
    // >> 3번째 옵션 flag : 해당 명령어 라인 옵션이 전달될때, 
    // 해당 변수에 값이 대입된다. 
    // 예를 들어 --debug 옵션이 전달되면, debug_mode 변수에 1이 대입된다.
    {"debug",  no_argument,       &debug_mode, 1},

    {"chroot", no_argument,       NULL, 'c'},

    {"user",   required_argument, NULL, 'u'},

    {"group",  required_argument, NULL, 'g'},

    {"port",   required_argument, NULL, 'p'},

    // 도움 메세지 내놓는 옵션
    {"help",   no_argument,       NULL, 'h'},  

    {0, 0, 0, 0}
};

int
main(int argc, char* argv[])
{
    int server_fd;
    char* port = NULL;
    char* docroot;
    int do_chroot = 0;
    char* user = NULL;
    char* group = NULL;
    int opt;

    while ((opt = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (opt) {
        case 0:
            break;
        case 'c':
            do_chroot = 1;
            break;
        case 'u':
            user = optarg;
            break;
        case 'g':
            group = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'h':
            fprintf(stdout, USAGE, argv[0]);
            exit(0);
        case '?':
            fprintf(stderr, USAGE, argv[0]);
            exit(1);
        }
    }
    if (optind != argc - 1) {
        fprintf(stderr, USAGE, argv[0]);
        exit(1);
    }

    // 여기까지가 실행 인자의 해석이다.
    docroot = argv[optind];

    // 환경 설정
    if (do_chroot) {
        setup_environment(docroot, user, group);
        docroot = "";
    }
    install_signal_handlers();

    // server_fd : 소켓을 나타내는 파일 디스크립터
    server_fd = listen_socket(port);

    if (!debug_mode) {
    
        /*
        * ex)  syslog 만 사용해도 로깅을 할 수 있으나, 로그의 이름등을 지정해부려면 openlog 를 호출한다.
        * 1번째 인자 : 로그에 출력되는 프로그램 이름
        * 2번째 인자 : 로그 옵션
        * ex) LOG_PID : 로그 메세지에 프로세스 ID 를 출력
        * ex) LOG_NDELAY : 로그 메세지를 즉시 출력
        * ex) LOG_DAEMON : 로그 메세지를 데몬 로그에 출력
        */
        openlog(SERVER_NAME, LOG_PID | LOG_NDELAY, LOG_DAEMON);
        
        become_daemon();
    }

    // 서버 시작
    server_main(server_fd, docroot);
    exit(0);
}

static void
setup_environment(char* root, char* user, char* groupParam)
{
    /*
    * 1) 인증을 user 와 gruop 에 변경
    * 2) chroot 을 실행.
    * chroot 은 슈퍼 사용자가 아니면 실행 불가능 하지만
    * HTTP 서버를 슈퍼 사용자의 상태로 실행하는 것은 위험하다.
    * 따라서 처음에는 슈퍼 사용자 권한으로 실행하고
    * chroot 이후에 스스로 권한을 포기할 것이다.
    */
    struct passwd* pw;
    struct group* gr;

    if (!user || !groupParam) {
        // 만약 둘 중 하나라도 없다면
        // --user, --group 옵션을 명령어 인자를 사용하지 않았다고 에러 출력
        fprintf(stderr, "use both of --user and --group\n");
        exit(1);
    }

    // 그룹 이름을 통해서 그룹 정보 얻기 
    gr = getgrnam(groupParam);
    if (!gr) {
        fprintf(stderr, "no such group: %s\n", groupParam);
        exit(1);
    }

    // 현재 프로세스의 그룹 id 를 gr_gid 로 세팅
    if (setgid(gr->gr_gid) < 0) {
        perror("setgid(2)");
        exit(1);
    }

    // 프로세스의 부가 그룹 id 추가
    // user : user 의 이름
    // gr_gid : primary groud id
    if (initgroups(user, gr->gr_gid) < 0) {
        perror("initgroups(2)");
        exit(1);
    }

    // user 이름을 사용하여 user 정보를 얻어오기
    pw = getpwnam(user);
    if (!pw) {
        fprintf(stderr, "no such user: %s\n", user);
        exit(1);
    }

    /*
    * HTTP URL 에서 파일 시스템의 파일을 얻는 처리 과정에서
    * 문서 트리 밖의 파일에 접근하는 것을 막기 위해 chroot 을 사용한다.
    * 프로세스가 볼 수있는 파일 시스템 자체를 좁히는 것이다.
    * 
    * >> chroot : 현재 프로세스의 루트 디렉터리를 path 로 변경한다.
    * 즉 chroot 을 호출한 이후 '/' 에 access 하면 새로운 루트
    * 디렉터리에 접근하게 된다.
    * 
    * >> chroot 을 하고나서 setgid, initgroups 를 호출하면 안될까 ?
    * 곤란하다. chroot 환경 속에서는 /etc 가 없기 때문이다.
    * /etc 가 없다는 것은 /etc/passwd 도 /etc/group 도 없다는 의미가 된다.
    * 그러면 getpwname, getgrnam, initgroups 도 동작하기 않는다.
    * /etc 가 필요한 함수들은 chroot 이전에 호출한다.
    */
    chroot(root);
    
    /*
    * chroot 은 슈퍼 사용자만 실행할 수 있다.
    * 따라서 먼서 슈퍼 사용자로 실행 된 후 프로그램 내에서
    * 다른 자격 증명서로 변경한다.
    * 
    * 그러므로 chroot 이전에 setuid 를 설정해서는 안된다.
    */

    /*
    * (참고) set-uid 비트. 라는 것이 있다.
    * 특정 프로그램에 대해서 set-uid 비트를 설정해두면
    * 실행한 사용자와 관계없이 프로그램 파일의 소유자 권한으로
    * 실행된다.
    * 
    * setuid 프로그램으로부터 동작한 프로세스에는 
    * 2가지 종류의 인증이 존재한다.
    * 1) 실제 사용자 : 프로세스를 시작한 사용자
    * 2) 실효 사용자 : set-uid 프로그램 소유자 id
    * 
    * (참고) 그룹에 대해서도 비슷한 구조가 있다. 
    * set-gid 비트 라는 것이 존재한다.
    * 
    * 1) 실제 그룹 id : 프로그램을 시작한 사용자 그룹 id
    * 2) 프로그램 소유 그룹 id : 실효 그룹 id
    */

    /*
    * (참고) 현재 권한을 버리고, 새로운 자격 증명으로 변경하려면
    * setuid, setgid, initgroups 를 사용한다.
    * 
    * >> setuid : 현재 프로세스의 실제 사용자 id, 실효 사용자 id 를
    * id 로 변경
    * 
    * >> setgid : 현재 프로세스의 실제 그룹 id, 실효 그룹 id 를 id 로 변경
    * 
    * >> initgroups : /etc/group 등의 데이터베이스를 보고
    * user 가 속한 보조 그룹을 현재 프로세스의 보조 그룹으로 설정.
    * 
    * 2번째 인자  group : 사용자 그룹(primary group) 을
    * 보조 그룹에도 추가하기 위해 사용
    * 
    * initgroups 는 슈퍼 사용자가 아니면 성공할 수 없다.
    */

    /*
    * 완전히 다른 사용자가 되기 위한 호출 순서
    * 1) 슈퍼 사용자로 프로그램 시작
    * 2) 원하는 사용자의 사용자명, ID, 그룹 ID 를 얻어온다.
    * 3) setgid  (변경할 그룹 ID) ->> 4) 와 순서 변경되어도 상관 X
    * 4) initgroups(변경할 사용자면, 그룹 ID)
    * 5) setuid (변경할 사용자 ID) --> 반드시 마지막에
    * 왜냐하면 initgroups 는 슈퍼 사용자로 실행해야 하기 때문
    */

    if (setuid(pw->pw_uid) < 0) {
        perror("setuid(2)");
        exit(1);
    }
}

static void
become_daemon(void)
{
    /*
    *   해당 함수는 크게 3가지 작업을 한다.
    * 1) 루트 디렉터리에 chdir 한다.
    * - chdir : 현재 작업 디렉터리를 변경하는 함수
    * - '/': 루트 디렉터리로 변경
    * - 파일 시스템이 마운트 해제할 수 없게 되는 것을 방지하기 위함
    * - 프로세스가 current_directory 로 사용하고 있는 파일 시스템은
    *    마운트 해제할 수 없다. 장시간 실행되는 데몬은 가급적 루트 디렉터리로 이동
    * - (참고) 마운트 해제 ?
    * 리눅스에서 파일 시스템 마운트 해제는 마치 특정 드라이브나 장치를 컴퓨터에서 분리하는 것과 같습니다. 
    * 마운트된 파일 시스템은 마치 외장 하드 드라이브나 USB 드라이브처럼 컴퓨터 시스템의 일부로 작동하며, 
    * 사용자는 마치 로컬 디렉토리와 마찬가지로 해당 파일 시스템에 있는 파일에 액세스하고 작업할 수 있습니다.
    * 파일 시스템 마운트 해제를 하면 다음과 같은 일이 발생합니다.
    * 파일 시스템과 컴퓨터 시스템의 연결이 끊어집니다. 마치 USB 드라이브를 컴퓨터에서 뽑아내는 것과 같습니다.
    * 사용자는 더 이상 마운트된 파일 시스템에 있는 파일에 액세스할 수 없습니다. 마치 USB 드라이브가 없는 것과 같습니다.
    * 마운트된 파일 시스템에 대한 모든 프로세스가 종료됩니다. 해당 파일 시스템에 액세스하는 프로그램이나 서비스가 모두 종료됩니다.
    * 
    * 2) 표준 입출력을 /dev/null 에 연결
    * - 리눅스에서 표준 입출력을 /dev/null에 연결한다는 것은 프로그램의 출력을 버리고 아무것도 표시하지 않도록 설정하는 것을 의미합니다.
    * /dev/null은 특수 장치 파일로, 모든 데이터가 입력되면 
    * 소멸되는 "블랙홀" 역할을 합니다. 
    * 프로그램의 표준 입출력을 /dev/null에 연결하면 
    * 마치 프로그램이 아무것도 출력하지 않는 것처럼 동작하게 됩니다.
    * 
    * - 프로그램이 무심코 표준 입출력을 사용할 때, 에러가 발생하지 않도록 하기 위함
    * 
    * ex) freopen("/dev/null", "r", stdin);
    * 
    * 3) 제어 단말을 잘라낸다. 
    * ex) setsid
    */
    int n;

    if (chdir("/") < 0)
        log_exit("chdir(2) failed: %s", strerror(errno));

    // freopen 
    /*
    * freopen("/dev/null", "r", stdin);
    * freopen: This function is used to reopen a previously opened file or stream.
    * "/dev/null": This argument specifies the path to a special device file called /dev/null.
    * "r": This argument specifies the mode in which the file is opened. Here, "r" stands for "read" mode.
    * In essence, this line attempts to reopen the standard input stream (stdin) and associates it with the /dev/null device.
    * 
    * What is /dev/null?
    * /dev/null is a special file in Unix-based systems (including Linux) that acts like a black hole. Any data written to /dev/null is discarded and disappears.
    * 
    * Why redirect stdin to /dev/null?
    * Redirecting stdin to /dev/null essentially disables any user input for the program. Any attempt to read from stdin will receive no data (like an empty void). This might be useful in situations where the program doesn't require user interaction or relies solely on pre-defined data or configuration files.
    */
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    n = fork();
    if (n < 0) log_exit("fork(2) failed: %s", strerror(errno));

    // fork() 함수에 대해서는 exit 함수가 아니라, _exit 을 사용한다.
    // exit 은 모든 FILE 대해 fflush 를 실행하여 하나의 FILE 이
    // 부모와 자식 프로세스 양쪽에서 fflush 가 된다.
    if (n != 0) _exit(0);

    /*
    * set_sid : 새로운 session 을 만들어내기 위한 함수
    * >> 1) 해당 함수를 실행한 프로세스는 session 의 leader 가 된다.
    * 세션이란 공통 속성 (ex. 제어 단말) 등을 공유하는 프로세스들의 집합이다.
    * >> 2) 제어 단말로부터 분리된다.
    * >> 3) 새로운 세션 안에 새로운 프로세스 그룹도 생성한다.
    * 당연히 현재 프로세스가 그 그룹의 leader 가 된다.
    */
    if (setsid() < 0) log_exit("setsid(2) failed: %s", strerror(errno));
}

static void
install_signal_handlers(void)
{
    trap_signal(SIGTERM, signal_exit);
    detach_children();
}

static void
trap_signal(int sig, sighandler_t handler)
{
    struct sigaction act;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if (sigaction(sig, &act, NULL) < 0) {
        log_exit("sigaction() failed: %s", strerror(errno));
    }
}

static void
detach_children(void)
{
    /*
    * fork 한 자식 프로세스는 wait 하지 않으면 좀비 프로세스가 될 수 있따.
    * 하지만 wait 만 하면, 자식 프로세스가 종료될 때까지 메인 프로세스에서
    * 아무것도 못하고 대기한다.
    * 
    * 이를 해결하는 2가지 방법이 있다.
    * 1) 시그널 SIGCHILD 를 수신할 때 wait 호출
    * 2) wait 를 하지 않게 한다.
    * 
    * 
    * 1) 자식 프로세스 종료되면 커널이 SIGCHILD 를 송신한다.
    * 해당 시그널 핸들로로 wait 하면, 즉시 성공한다.
    * 따라서 쓸데없이 wait 로 기다릴 필요가 없다.
    * 
    * 2) sigaction 을 사용해서 wait 을 안할 것이라고 커널에 선언
    * SA_NOCLDWAIT 플래그를 시그널 SIGCHILD 와 함께 sigaction 에
    * 전달하면, 커널은 "이 프로세느는 이제 더 wait() 하는 일이 없다." 라고 판단.
    * 자식 프로세스를 좀비로 만들지 않는다.
    * 
    * 그 이후 wait 은 에러가 되므로 주의해야 한다.
    */
    struct sigaction act;

    // 시그널 핸들로로, 아무것도 하지않는 noop_handler 를 등록한다.
    act.sa_handler = noop_handler;
    sigemptyset(&act.sa_mask);
    // SA_NOCLDWAIT 플래그 세팅 후 sigaction 호출
    act.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    if (sigaction(SIGCHLD, &act, NULL) < 0) {
        log_exit("sigaction() failed: %s", strerror(errno));
    }
}

static void
signal_exit(int sig)
{
    log_exit("exit by signal %d", sig);
}

static void
noop_handler(int sig)
{
    ;
}

// socket -> listen 까지 담당
static int
listen_socket(char* port)
{
    struct addrinfo hints, * res, * ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    // AF_INET : IPv4 주소체계에 한정.
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // AI_PASSIVE : socket 을 서버용으로 사용하는 것 지시.
    hints.ai_flags = AI_PASSIVE;

    // >> 서버측에서 getaddrinfo 을 사용하면, 프로세스가 동작하고 있는
    // 호스트의 address 구조체를 얻는다.
    // >> (참고) 클라이언트가 소켓에 접속할 때는, 상대 호스트의 주소를
    // 얻을 때 사용
    if ((err = getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        log_exit(gai_strerror(err));
    }

    // 사실, 일반적으로 getaddrinfo 를 통해서는 로컬 호스트의
    // IPV4 주소 하나만 얻는다. 루프로 처리할 필요는 없지만
    // 일반적인 경우를 커버하기 위한 로직이다.
    for (ai = res; ai; ai = ai->ai_next) {
        int sock;

        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock < 0) continue;

        // 주소 할당
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        if (listen(sock, MAX_BACKLOG) < 0) {
            close(sock);
            continue;
        }
        freeaddrinfo(res);

        // 성공한 최초의 address 를 이용하여 서버 소켓 생성하고 break
        return sock;
    }
    log_exit("failed to listen socket");
    return -1;  /* NOT REACH */
}

static void
server_main(int server_fd, char* docroot)
{
    for (;;) {
        // sockaddr_storage : IP 주소와 포트 번호를 포함한 구조체
        // struct socketaddr 을 확보할 때 사용하는 전용 구조체
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof addr;
        int sock;
        int pid;

        // 클라이언트의 접속을 기다린다.
        // 연결이 완료되면 새로운 소켓을 반환한다.
        // (참고) listen 까지는 소켓에 스트림이 연결 X
        //          accept 를 통해 연결이 완료되면, 스트림이 연결된다.
        // (참고) socket, bind, listen 은 프로세스당 1회만 호출
        //          accept 는 여러번 호출 가능
        sock = accept(server_fd, (struct sockaddr*)&addr, &addrlen);

        if (sock < 0) log_exit("accept(2) failed: %s", strerror(errno));

        // 연결을 수락한 이후에는, 프로세스를 나눠서 복수의 클라이언트를
        // 동시에 상대할 것이다. 즉, 복수 프로세스로 처리할 것이다.
        // fork : 자식 프로세스를 만드는 함수.
        pid = fork();

        if (pid < 0) exit(3);
        if (pid == 0) {   /* child */
            FILE* inf = fdopen(sock, "r");
            FILE* outf = fdopen(sock, "w");
            // service : 요청을 처리하는 함수
            service(inf, outf, docroot);
            // 처리가 끝나면 exit 한다.
            exit(0);
        }
        // accet 를 통해 얻은 소켓을 close 한다.
        // 1) 해당 함수를 호출하지 않으면, 연결된 소켓이
        // 부모 프로세스에 쌓인다.
        // 2) close 를 해야 연결이 끊긴다. 그렇지 않으면
        // 클라이언트는 언제까지 기다리게된다.
        close(sock);
    }
}

static void
service(FILE* in, FILE* out, char* docroot)
{
    struct HTTPRequest* req;

    req = read_request(in);
    respond_to(req, out, docroot);
    free_request(req);
}

static struct HTTPRequest*
read_request(FILE* in)
{
    struct HTTPRequest* req;
    struct HTTPHeaderField* h;

    req = xmalloc(sizeof(struct HTTPRequest));
    read_request_line(req, in);
    req->header = NULL;
    while (h = read_header_field(in)) {
        h->next = req->header;
        req->header = h;
    }
    req->length = content_length(req);
    if (req->length != 0) {
        if (req->length > MAX_REQUEST_BODY_LENGTH)
            log_exit("request body too long");
        req->body = xmalloc(req->length);
        if (fread(req->body, req->length, 1, in) < 1)
            log_exit("failed to read request body");
    }
    else {
        req->body = NULL;
    }
    return req;
}

static void
read_request_line(struct HTTPRequest* req, FILE* in)
{
    char buf[LINE_BUF_SIZE];
    char* path, * p;

    if (!fgets(buf, LINE_BUF_SIZE, in))
        log_exit("no request line");
    p = strchr(buf, ' ');       /* p (1) */
    if (!p) log_exit("parse error on request line (1): %s", buf);
    *p++ = '\0';
    req->method = xmalloc(p - buf);
    strcpy(req->method, buf);
    upcase(req->method);

    path = p;
    p = strchr(path, ' ');      /* p (2) */
    if (!p) log_exit("parse error on request line (2): %s", buf);
    *p++ = '\0';
    req->path = xmalloc(p - path);
    strcpy(req->path, path);

    if (strncasecmp(p, "HTTP/1.", strlen("HTTP/1.")) != 0)
        log_exit("parse error on request line (3): %s", buf);
    p += strlen("HTTP/1.");     /* p (3) */
    req->protocol_minor_version = atoi(p);
}

static struct HTTPHeaderField*
read_header_field(FILE* in)
{
    struct HTTPHeaderField* h;
    char buf[LINE_BUF_SIZE];
    char* p;

    if (!fgets(buf, LINE_BUF_SIZE, in))
        log_exit("failed to read request header field: %s", strerror(errno));
    if ((buf[0] == '\n') || (strcmp(buf, "\r\n") == 0))
        return NULL;

    p = strchr(buf, ':');
    if (!p) log_exit("parse error on request header field: %s", buf);
    *p++ = '\0';
    h = xmalloc(sizeof(struct HTTPHeaderField));
    h->name = xmalloc(p - buf);
    strcpy(h->name, buf);

    p += strspn(p, " \t");
    h->value = xmalloc(strlen(p) + 1);
    strcpy(h->value, p);

    return h;
}

static void
upcase(char* str)
{
    char* p;

    for (p = str; *p; p++) {
        *p = (char)toupper((int)*p);
    }
}

static void
free_request(struct HTTPRequest* req)
{
    struct HTTPHeaderField* h, * head;

    head = req->header;
    while (head) {
        h = head;
        head = head->next;
        free(h->name);
        free(h->value);
        free(h);
    }
    free(req->method);
    free(req->path);
    free(req->body);
    free(req);
}

static long
content_length(struct HTTPRequest* req)
{
    char* val;
    long len;

    val = lookup_header_field_value(req, "Content-Length");
    if (!val) return 0;
    len = atol(val);
    if (len < 0) log_exit("negative Content-Length value");
    return len;
}

static char*
lookup_header_field_value(struct HTTPRequest* req, char* name)
{
    struct HTTPHeaderField* h;

    for (h = req->header; h; h = h->next) {
        if (strcasecmp(h->name, name) == 0)
            return h->value;
    }
    return NULL;
}

static void
respond_to(struct HTTPRequest* req, FILE* out, char* docroot)
{
    if (strcmp(req->method, "GET") == 0)
        do_file_response(req, out, docroot);
    else if (strcmp(req->method, "HEAD") == 0)
        do_file_response(req, out, docroot);
    else if (strcmp(req->method, "POST") == 0)
        method_not_allowed(req, out);
    else
        not_implemented(req, out);
}

static void
do_file_response(struct HTTPRequest* req, FILE* out, char* docroot)
{
    struct FileInfo* info;

    info = get_fileinfo(docroot, req->path);
    if (!info->ok) {
        free_fileinfo(info);
        not_found(req, out);
        return;
    }
    output_common_header_fields(req, out, "200 OK");
    fprintf(out, "Content-Length: %ld\r\n", info->size);
    fprintf(out, "Content-Type: %s\r\n", guess_content_type(info));
    fprintf(out, "\r\n");
    if (strcmp(req->method, "HEAD") != 0) {
        int fd;
        char buf[BLOCK_BUF_SIZE];
        ssize_t n;

        fd = open(info->path, O_RDONLY);
        if (fd < 0)
            log_exit("failed to open %s: %s", info->path, strerror(errno));
        for (;;) {
            n = read(fd, buf, BLOCK_BUF_SIZE);
            if (n < 0)
                log_exit("failed to read %s: %s", info->path, strerror(errno));
            if (n == 0)
                break;
            if (fwrite(buf, 1, n, out) < n)
                log_exit("failed to write to socket");
        }
        close(fd);
    }
    fflush(out);
    free_fileinfo(info);
}

static void
method_not_allowed(struct HTTPRequest* req, FILE* out)
{
    output_common_header_fields(req, out, "405 Method Not Allowed");
    fprintf(out, "Content-Type: text/html\r\n");
    fprintf(out, "\r\n");
    fprintf(out, "<html>\r\n");
    fprintf(out, "<header>\r\n");
    fprintf(out, "<title>405 Method Not Allowed</title>\r\n");
    fprintf(out, "<header>\r\n");
    fprintf(out, "<body>\r\n");
    fprintf(out, "<p>The request method %s is not allowed</p>\r\n", req->method);
    fprintf(out, "</body>\r\n");
    fprintf(out, "</html>\r\n");
    fflush(out);
}

static void
not_implemented(struct HTTPRequest* req, FILE* out)
{
    output_common_header_fields(req, out, "501 Not Implemented");
    fprintf(out, "Content-Type: text/html\r\n");
    fprintf(out, "\r\n");
    fprintf(out, "<html>\r\n");
    fprintf(out, "<header>\r\n");
    fprintf(out, "<title>501 Not Implemented</title>\r\n");
    fprintf(out, "<header>\r\n");
    fprintf(out, "<body>\r\n");
    fprintf(out, "<p>The request method %s is not implemented</p>\r\n", req->method);
    fprintf(out, "</body>\r\n");
    fprintf(out, "</html>\r\n");
    fflush(out);
}

static void
not_found(struct HTTPRequest* req, FILE* out)
{
    output_common_header_fields(req, out, "404 Not Found");
    fprintf(out, "Content-Type: text/html\r\n");
    fprintf(out, "\r\n");
    if (strcmp(req->method, "HEAD") != 0) {
        fprintf(out, "<html>\r\n");
        fprintf(out, "<header><title>Not Found</title><header>\r\n");
        fprintf(out, "<body><p>File not found</p></body>\r\n");
        fprintf(out, "</html>\r\n");
    }
    fflush(out);
}

#define TIME_BUF_SIZE 64

static void
output_common_header_fields(struct HTTPRequest* req, FILE* out, char* status)
{
    time_t t;
    struct tm* tm;
    char buf[TIME_BUF_SIZE];

    t = time(NULL);
    tm = gmtime(&t);
    if (!tm) log_exit("gmtime() failed: %s", strerror(errno));
    strftime(buf, TIME_BUF_SIZE, "%a, %d %b %Y %H:%M:%S GMT", tm);
    fprintf(out, "HTTP/1.%d %s\r\n", HTTP_MINOR_VERSION, status);
    fprintf(out, "Date: %s\r\n", buf);
    fprintf(out, "Server: %s/%s\r\n", SERVER_NAME, SERVER_VERSION);
    fprintf(out, "Connection: close\r\n");
}

static struct FileInfo*
get_fileinfo(char* docroot, char* urlpath)
{
    struct FileInfo* info;
    struct stat st;

    info = xmalloc(sizeof(struct FileInfo));
    info->path = build_fspath(docroot, urlpath);
    info->ok = 0;
    if (lstat(info->path, &st) < 0) return info;
    if (!S_ISREG(st.st_mode)) return info;
    info->ok = 1;
    info->size = st.st_size;
    return info;
}

static char*
build_fspath(char* docroot, char* urlpath)
{
    char* path;

    path = xmalloc(strlen(docroot) + 1 + strlen(urlpath) + 1);
    sprintf(path, "%s/%s", docroot, urlpath);
    return path;
}

static void
free_fileinfo(struct FileInfo* info)
{
    free(info->path);
    free(info);
}

static char*
guess_content_type(struct FileInfo* info)
{
    return "text/plain";   /* FIXME */
}

static void*
xmalloc(size_t sz)
{
    void* p;

    p = malloc(sz);
    if (!p) log_exit("failed to allocate memory");
    return p;
}

static void
log_exit(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    if (debug_mode) {
        /*
        *  >> debug_mode == 1 인 경우는, 데몬화 하지 않으므로
        * 표준 에러 출력을 그대로 사용한다.
        */
        vfprintf(stderr, fmt, ap);
        fputc('\n', stderr);
    }
    else {
        /*
        *  Q. 데몬이 되었을 때, 에러 메세지는 어디에 출력하는가 ?
        * 데몬은 단말로부터 완전히 분리하여 작동해야 하므로
        * 표준 입출력을 사용할 수 없다.
        * 
        * 통상적으로는 특정 파일에 메세지를 넣는 것이고
        * 이런 종류의 파일을 "로그파일" 이라고 한다.
        */
        /*
        *   syslog 함수
        * - 1번째 인자 : facility 플래그.
        * ex) LOG_ERR : 일반 에러
        */
        vsyslog(LOG_ERR, fmt, ap);
    }
    va_end(ap);
    exit(1);
}