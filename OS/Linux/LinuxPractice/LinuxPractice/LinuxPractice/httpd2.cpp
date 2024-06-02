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

// ª�� �ɼ��� �ƴ϶�, �� �ɼ��� ����Ѵ�.
static struct option longopts[] = {
    // >> ���α׷��� ����� ���� �����ϴ� �ɼ�
    // 
    // >> ����� ��忡���� ������ ���� �ʰ�, ǥ�� ������� �ܸ��� ������ ���·� ����ȴ�.
    // �̸� ���� ���� �޼����� ǥ�� ���� ��¿� ����� �� �ִ�.
    // 
    // >> 3��° �ɼ� flag : �ش� ��ɾ� ���� �ɼ��� ���޵ɶ�, 
    // �ش� ������ ���� ���Եȴ�. 
    // ���� ��� --debug �ɼ��� ���޵Ǹ�, debug_mode ������ 1�� ���Եȴ�.
    {"debug",  no_argument,       &debug_mode, 1},

    {"chroot", no_argument,       NULL, 'c'},

    {"user",   required_argument, NULL, 'u'},

    {"group",  required_argument, NULL, 'g'},

    {"port",   required_argument, NULL, 'p'},

    // ���� �޼��� ������ �ɼ�
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

    // ��������� ���� ������ �ؼ��̴�.
    docroot = argv[optind];

    // ȯ�� ����
    if (do_chroot) {
        setup_environment(docroot, user, group);
        docroot = "";
    }
    install_signal_handlers();

    // server_fd : ������ ��Ÿ���� ���� ��ũ����
    server_fd = listen_socket(port);

    if (!debug_mode) {
    
        /*
        * ex)  syslog �� ����ص� �α��� �� �� ������, �α��� �̸����� �����غη��� openlog �� ȣ���Ѵ�.
        * 1��° ���� : �α׿� ��µǴ� ���α׷� �̸�
        * 2��° ���� : �α� �ɼ�
        * ex) LOG_PID : �α� �޼����� ���μ��� ID �� ���
        * ex) LOG_NDELAY : �α� �޼����� ��� ���
        * ex) LOG_DAEMON : �α� �޼����� ���� �α׿� ���
        */
        openlog(SERVER_NAME, LOG_PID | LOG_NDELAY, LOG_DAEMON);
        
        become_daemon();
    }

    // ���� ����
    server_main(server_fd, docroot);
    exit(0);
}

static void
setup_environment(char* root, char* user, char* groupParam)
{
    /*
    * 1) ������ user �� gruop �� ����
    * 2) chroot �� ����.
    * chroot �� ���� ����ڰ� �ƴϸ� ���� �Ұ��� ������
    * HTTP ������ ���� ������� ���·� �����ϴ� ���� �����ϴ�.
    * ���� ó������ ���� ����� �������� �����ϰ�
    * chroot ���Ŀ� ������ ������ ������ ���̴�.
    */
    struct passwd* pw;
    struct group* gr;

    if (!user || !groupParam) {
        // ���� �� �� �ϳ��� ���ٸ�
        // --user, --group �ɼ��� ��ɾ� ���ڸ� ������� �ʾҴٰ� ���� ���
        fprintf(stderr, "use both of --user and --group\n");
        exit(1);
    }

    // �׷� �̸��� ���ؼ� �׷� ���� ��� 
    gr = getgrnam(groupParam);
    if (!gr) {
        fprintf(stderr, "no such group: %s\n", groupParam);
        exit(1);
    }

    // ���� ���μ����� �׷� id �� gr_gid �� ����
    if (setgid(gr->gr_gid) < 0) {
        perror("setgid(2)");
        exit(1);
    }

    // ���μ����� �ΰ� �׷� id �߰�
    // user : user �� �̸�
    // gr_gid : primary groud id
    if (initgroups(user, gr->gr_gid) < 0) {
        perror("initgroups(2)");
        exit(1);
    }

    // user �̸��� ����Ͽ� user ������ ������
    pw = getpwnam(user);
    if (!pw) {
        fprintf(stderr, "no such user: %s\n", user);
        exit(1);
    }

    /*
    * HTTP URL ���� ���� �ý����� ������ ��� ó�� ��������
    * ���� Ʈ�� ���� ���Ͽ� �����ϴ� ���� ���� ���� chroot �� ����Ѵ�.
    * ���μ����� �� ���ִ� ���� �ý��� ��ü�� ������ ���̴�.
    * 
    * >> chroot : ���� ���μ����� ��Ʈ ���͸��� path �� �����Ѵ�.
    * �� chroot �� ȣ���� ���� '/' �� access �ϸ� ���ο� ��Ʈ
    * ���͸��� �����ϰ� �ȴ�.
    * 
    * >> chroot �� �ϰ��� setgid, initgroups �� ȣ���ϸ� �ȵɱ� ?
    * ����ϴ�. chroot ȯ�� �ӿ����� /etc �� ���� �����̴�.
    * /etc �� ���ٴ� ���� /etc/passwd �� /etc/group �� ���ٴ� �ǹ̰� �ȴ�.
    * �׷��� getpwname, getgrnam, initgroups �� �����ϱ� �ʴ´�.
    * /etc �� �ʿ��� �Լ����� chroot ������ ȣ���Ѵ�.
    */
    chroot(root);
    
    /*
    * chroot �� ���� ����ڸ� ������ �� �ִ�.
    * ���� �ռ� ���� ����ڷ� ���� �� �� ���α׷� ������
    * �ٸ� �ڰ� ������ �����Ѵ�.
    * 
    * �׷��Ƿ� chroot ������ setuid �� �����ؼ��� �ȵȴ�.
    */

    /*
    * (����) set-uid ��Ʈ. ��� ���� �ִ�.
    * Ư�� ���α׷��� ���ؼ� set-uid ��Ʈ�� �����صθ�
    * ������ ����ڿ� ������� ���α׷� ������ ������ ��������
    * ����ȴ�.
    * 
    * setuid ���α׷����κ��� ������ ���μ������� 
    * 2���� ������ ������ �����Ѵ�.
    * 1) ���� ����� : ���μ����� ������ �����
    * 2) ��ȿ ����� : set-uid ���α׷� ������ id
    * 
    * (����) �׷쿡 ���ؼ��� ����� ������ �ִ�. 
    * set-gid ��Ʈ ��� ���� �����Ѵ�.
    * 
    * 1) ���� �׷� id : ���α׷��� ������ ����� �׷� id
    * 2) ���α׷� ���� �׷� id : ��ȿ �׷� id
    */

    /*
    * (����) ���� ������ ������, ���ο� �ڰ� �������� �����Ϸ���
    * setuid, setgid, initgroups �� ����Ѵ�.
    * 
    * >> setuid : ���� ���μ����� ���� ����� id, ��ȿ ����� id ��
    * id �� ����
    * 
    * >> setgid : ���� ���μ����� ���� �׷� id, ��ȿ �׷� id �� id �� ����
    * 
    * >> initgroups : /etc/group ���� �����ͺ��̽��� ����
    * user �� ���� ���� �׷��� ���� ���μ����� ���� �׷����� ����.
    * 
    * 2��° ����  group : ����� �׷�(primary group) ��
    * ���� �׷쿡�� �߰��ϱ� ���� ���
    * 
    * initgroups �� ���� ����ڰ� �ƴϸ� ������ �� ����.
    */

    /*
    * ������ �ٸ� ����ڰ� �Ǳ� ���� ȣ�� ����
    * 1) ���� ����ڷ� ���α׷� ����
    * 2) ���ϴ� ������� ����ڸ�, ID, �׷� ID �� ���´�.
    * 3) setgid  (������ �׷� ID) ->> 4) �� ���� ����Ǿ ��� X
    * 4) initgroups(������ ����ڸ�, �׷� ID)
    * 5) setuid (������ ����� ID) --> �ݵ�� ��������
    * �ֳ��ϸ� initgroups �� ���� ����ڷ� �����ؾ� �ϱ� ����
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
    *   �ش� �Լ��� ũ�� 3���� �۾��� �Ѵ�.
    * 1) ��Ʈ ���͸��� chdir �Ѵ�.
    * - chdir : ���� �۾� ���͸��� �����ϴ� �Լ�
    * - '/': ��Ʈ ���͸��� ����
    * - ���� �ý����� ����Ʈ ������ �� ���� �Ǵ� ���� �����ϱ� ����
    * - ���μ����� current_directory �� ����ϰ� �ִ� ���� �ý�����
    *    ����Ʈ ������ �� ����. ��ð� ����Ǵ� ������ ������ ��Ʈ ���͸��� �̵�
    * - (����) ����Ʈ ���� ?
    * ���������� ���� �ý��� ����Ʈ ������ ��ġ Ư�� ����̺곪 ��ġ�� ��ǻ�Ϳ��� �и��ϴ� �Ͱ� �����ϴ�. 
    * ����Ʈ�� ���� �ý����� ��ġ ���� �ϵ� ����̺곪 USB ����̺�ó�� ��ǻ�� �ý����� �Ϻη� �۵��ϸ�, 
    * ����ڴ� ��ġ ���� ���丮�� ���������� �ش� ���� �ý��ۿ� �ִ� ���Ͽ� �׼����ϰ� �۾��� �� �ֽ��ϴ�.
    * ���� �ý��� ����Ʈ ������ �ϸ� ������ ���� ���� �߻��մϴ�.
    * ���� �ý��۰� ��ǻ�� �ý����� ������ �������ϴ�. ��ġ USB ����̺긦 ��ǻ�Ϳ��� �̾Ƴ��� �Ͱ� �����ϴ�.
    * ����ڴ� �� �̻� ����Ʈ�� ���� �ý��ۿ� �ִ� ���Ͽ� �׼����� �� �����ϴ�. ��ġ USB ����̺갡 ���� �Ͱ� �����ϴ�.
    * ����Ʈ�� ���� �ý��ۿ� ���� ��� ���μ����� ����˴ϴ�. �ش� ���� �ý��ۿ� �׼����ϴ� ���α׷��̳� ���񽺰� ��� ����˴ϴ�.
    * 
    * 2) ǥ�� ������� /dev/null �� ����
    * - ���������� ǥ�� ������� /dev/null�� �����Ѵٴ� ���� ���α׷��� ����� ������ �ƹ��͵� ǥ������ �ʵ��� �����ϴ� ���� �ǹ��մϴ�.
    * /dev/null�� Ư�� ��ġ ���Ϸ�, ��� �����Ͱ� �ԷµǸ� 
    * �Ҹ�Ǵ� "��Ȧ" ������ �մϴ�. 
    * ���α׷��� ǥ�� ������� /dev/null�� �����ϸ� 
    * ��ġ ���α׷��� �ƹ��͵� ������� �ʴ� ��ó�� �����ϰ� �˴ϴ�.
    * 
    * - ���α׷��� ������ ǥ�� ������� ����� ��, ������ �߻����� �ʵ��� �ϱ� ����
    * 
    * ex) freopen("/dev/null", "r", stdin);
    * 
    * 3) ���� �ܸ��� �߶󳽴�. 
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

    // fork() �Լ��� ���ؼ��� exit �Լ��� �ƴ϶�, _exit �� ����Ѵ�.
    // exit �� ��� FILE ���� fflush �� �����Ͽ� �ϳ��� FILE ��
    // �θ�� �ڽ� ���μ��� ���ʿ��� fflush �� �ȴ�.
    if (n != 0) _exit(0);

    /*
    * set_sid : ���ο� session �� ������ ���� �Լ�
    * >> 1) �ش� �Լ��� ������ ���μ����� session �� leader �� �ȴ�.
    * �����̶� ���� �Ӽ� (ex. ���� �ܸ�) ���� �����ϴ� ���μ������� �����̴�.
    * >> 2) ���� �ܸ��κ��� �и��ȴ�.
    * >> 3) ���ο� ���� �ȿ� ���ο� ���μ��� �׷쵵 �����Ѵ�.
    * �翬�� ���� ���μ����� �� �׷��� leader �� �ȴ�.
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
    * fork �� �ڽ� ���μ����� wait ���� ������ ���� ���μ����� �� �� �ֵ�.
    * ������ wait �� �ϸ�, �ڽ� ���μ����� ����� ������ ���� ���μ�������
    * �ƹ��͵� ���ϰ� ����Ѵ�.
    * 
    * �̸� �ذ��ϴ� 2���� ����� �ִ�.
    * 1) �ñ׳� SIGCHILD �� ������ �� wait ȣ��
    * 2) wait �� ���� �ʰ� �Ѵ�.
    * 
    * 
    * 1) �ڽ� ���μ��� ����Ǹ� Ŀ���� SIGCHILD �� �۽��Ѵ�.
    * �ش� �ñ׳� �ڵ�η� wait �ϸ�, ��� �����Ѵ�.
    * ���� �������� wait �� ��ٸ� �ʿ䰡 ����.
    * 
    * 2) sigaction �� ����ؼ� wait �� ���� ���̶�� Ŀ�ο� ����
    * SA_NOCLDWAIT �÷��׸� �ñ׳� SIGCHILD �� �Բ� sigaction ��
    * �����ϸ�, Ŀ���� "�� ���μ����� ���� �� wait() �ϴ� ���� ����." ��� �Ǵ�.
    * �ڽ� ���μ����� ����� ������ �ʴ´�.
    * 
    * �� ���� wait �� ������ �ǹǷ� �����ؾ� �Ѵ�.
    */
    struct sigaction act;

    // �ñ׳� �ڵ�η�, �ƹ��͵� �����ʴ� noop_handler �� ����Ѵ�.
    act.sa_handler = noop_handler;
    sigemptyset(&act.sa_mask);
    // SA_NOCLDWAIT �÷��� ���� �� sigaction ȣ��
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

// socket -> listen ���� ���
static int
listen_socket(char* port)
{
    struct addrinfo hints, * res, * ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    // AF_INET : IPv4 �ּ�ü�迡 ����.
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // AI_PASSIVE : socket �� ���������� ����ϴ� �� ����.
    hints.ai_flags = AI_PASSIVE;

    // >> ���������� getaddrinfo �� ����ϸ�, ���μ����� �����ϰ� �ִ�
    // ȣ��Ʈ�� address ����ü�� ��´�.
    // >> (����) Ŭ���̾�Ʈ�� ���Ͽ� ������ ����, ��� ȣ��Ʈ�� �ּҸ�
    // ���� �� ���
    if ((err = getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        log_exit(gai_strerror(err));
    }

    // ���, �Ϲ������� getaddrinfo �� ���ؼ��� ���� ȣ��Ʈ��
    // IPV4 �ּ� �ϳ��� ��´�. ������ ó���� �ʿ�� ������
    // �Ϲ����� ��츦 Ŀ���ϱ� ���� �����̴�.
    for (ai = res; ai; ai = ai->ai_next) {
        int sock;

        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock < 0) continue;

        // �ּ� �Ҵ�
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        if (listen(sock, MAX_BACKLOG) < 0) {
            close(sock);
            continue;
        }
        freeaddrinfo(res);

        // ������ ������ address �� �̿��Ͽ� ���� ���� �����ϰ� break
        return sock;
    }
    log_exit("failed to listen socket");
    return -1;  /* NOT REACH */
}

static void
server_main(int server_fd, char* docroot)
{
    for (;;) {
        // sockaddr_storage : IP �ּҿ� ��Ʈ ��ȣ�� ������ ����ü
        // struct socketaddr �� Ȯ���� �� ����ϴ� ���� ����ü
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof addr;
        int sock;
        int pid;

        // Ŭ���̾�Ʈ�� ������ ��ٸ���.
        // ������ �Ϸ�Ǹ� ���ο� ������ ��ȯ�Ѵ�.
        // (����) listen ������ ���Ͽ� ��Ʈ���� ���� X
        //          accept �� ���� ������ �Ϸ�Ǹ�, ��Ʈ���� ����ȴ�.
        // (����) socket, bind, listen �� ���μ����� 1ȸ�� ȣ��
        //          accept �� ������ ȣ�� ����
        sock = accept(server_fd, (struct sockaddr*)&addr, &addrlen);

        if (sock < 0) log_exit("accept(2) failed: %s", strerror(errno));

        // ������ ������ ���Ŀ���, ���μ����� ������ ������ Ŭ���̾�Ʈ��
        // ���ÿ� ����� ���̴�. ��, ���� ���μ����� ó���� ���̴�.
        // fork : �ڽ� ���μ����� ����� �Լ�.
        pid = fork();

        if (pid < 0) exit(3);
        if (pid == 0) {   /* child */
            FILE* inf = fdopen(sock, "r");
            FILE* outf = fdopen(sock, "w");
            // service : ��û�� ó���ϴ� �Լ�
            service(inf, outf, docroot);
            // ó���� ������ exit �Ѵ�.
            exit(0);
        }
        // accet �� ���� ���� ������ close �Ѵ�.
        // 1) �ش� �Լ��� ȣ������ ������, ����� ������
        // �θ� ���μ����� ���δ�.
        // 2) close �� �ؾ� ������ �����. �׷��� ������
        // Ŭ���̾�Ʈ�� �������� ��ٸ��Եȴ�.
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
        *  >> debug_mode == 1 �� ����, ����ȭ ���� �����Ƿ�
        * ǥ�� ���� ����� �״�� ����Ѵ�.
        */
        vfprintf(stderr, fmt, ap);
        fputc('\n', stderr);
    }
    else {
        /*
        *  Q. ������ �Ǿ��� ��, ���� �޼����� ��� ����ϴ°� ?
        * ������ �ܸ��κ��� ������ �и��Ͽ� �۵��ؾ� �ϹǷ�
        * ǥ�� ������� ����� �� ����.
        * 
        * ��������δ� Ư�� ���Ͽ� �޼����� �ִ� ���̰�
        * �̷� ������ ������ "�α�����" �̶�� �Ѵ�.
        */
        /*
        *   syslog �Լ�
        * - 1��° ���� : facility �÷���.
        * ex) LOG_ERR : �Ϲ� ����
        */
        vsyslog(LOG_ERR, fmt, ap);
    }
    va_end(ap);
    exit(1);
}