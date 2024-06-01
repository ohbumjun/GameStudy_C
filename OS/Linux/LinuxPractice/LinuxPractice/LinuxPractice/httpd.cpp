/*
    httpd.c

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
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>


/****** Function Prototypes **********************************************/

typedef void (*sighandler_t)(int);
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void signal_exit(int sig);
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
static void log_exit(char* fmt, ...);

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <docroot>\n", argv[0]);
        exit(1);
    }
    // 프로그램 인자 1개 : 문서 루트의 경로

    install_signal_handlers();

    // 표준 입력과 표준 출력을 지정
    service(stdin, stdout, argv[1]);

    exit(0);
}

static void
service(FILE* in, FILE* out, char* docroot)
{
    struct HTTPRequest* req;

    // in 스트림에서 HTTP 요청을 읽고 그 정보를 req 에 저장
    req = read_request(in);

    // req 에 대한 응답을 out 스트림에 써 넣는다.
    // 이때 참고하는 문서 루트가 docroot 이다.
    respond_to(req, out, docroot);

    // service 를 한번만 호출. 즉, 현재 http 서버는 하나의 요청을 받아서 처리하고 종료
    free_request(req);
}


static struct HTTPRequest* read_request(FILE* in)
{
    struct HTTPRequest* req;

    /*
    * HTTPHeaderField
    * - char* name
    * - char* value
    * - HTTPHeaderField* next;
     
    즉, HTTPHeaderField 는 연결리스트. 형태로 연결되어 있다.
    HTTPHeaderField 하나가, 헤더 필드 한개(이름, 값의 세트) 를 나타낸다.
    */
    struct HTTPHeaderField* h;

    req = xmalloc(sizeof(struct HTTPRequest));

    // HTTP 요청의 첫번째 줄에 해당하는 "요청 라인" 을 읽는다.
    // ex) GET/path/to/file HTTP/1.1
    read_request_line(req, in);

    req->header = NULL;                     // header : HTTP 헤더

    // 이어서 요청 헤더를 하나씩 읽어들일 것이다.
    // 그리고 그 정보를 HTTPHeaderField 로 만들어서 연결리스트에 추가한다.
    while (h = read_header_field(in)) {
        // 연결리스트를 뒤에서 앞 방향? 으로 연결을 해간다.
        h->next = req->header;
        req->header = h;
    }

    // 요청 헤더 부분을 모두 읽으면
    // 이후에는 body 부분을 읽어들일 것이다.
    // HTTP 요청에 엔티티 body 가 존재할 경우,
    // 클라이언트가 엔티티 Body 의 길이를 Content-Length 필드에 쓰게 되어 있어서, 그 정보를 참조한다.
    req->length = content_length(req);

    if (req->length != 0) 
    {
        if (req->length > MAX_REQUEST_BODY_LENGTH)
        {
            log_exit("request body too long");
        }

        // body : char* == 본문 char 을 가리키는 포인터
        req->body = xmalloc(req->length);

        if (fread(req->body, req->length, 1, in) < 1)
        {
            log_exit("failed to read request body");
        }
    }
    else 
    {
        req->body = NULL;
    }

    return req;
}


// HTTP 요청의 첫번째 줄에 해당하는 "요청 라인" 을 읽는다.
// ex) GET /path/to/file HTTP/1.0
static void read_request_line(struct HTTPRequest* req, FILE* in)
{
    char buf[LINE_BUF_SIZE];

    char* path, * p;

    // 일단 한줄을 읽는다.
    // 네트워크를 통해 전송되는 데이터는 신뢰할 수 없으므로 제한을 둬야 한다.
    // 그래서 한줄 최대 길이를 LINE_BUF_SIZE 로 제한한다.
    // buf 는 요청 라인 char 의 첫번째 문자열 주소를 가리킬 것이다.
    if (!fgets(buf, LINE_BUF_SIZE, in))
    {
        log_exit("no request line");
    }

    // Searches for the first occurrence of the space character (' ') in the string `buf` and assigns its address to `p`
    // 즉, 이 경우 공백 부분을 찾는다.
    p = strchr(buf, ' ');     

    if (!p) log_exit("parse error on request line (1): %s", buf);

    // 현재 가리키는 위치에 '\0' 을 대입하고 포인터 한칸 진행한다.
    // 기존 p 는 GET 다음 ' ' 라는 공백을 가리키고 있었다. 해당 ' ' 을 \0  으로 바꾸고 p 를 한칸 이동한다.
    // 즉, 그 다음 '/' 으로 이동한다고 생각하면 된다.
    *p++ = '\0';

    // method : 요청 메소드(GET, HEAD, POST) 등을 모두 "대문자" 로 저장한다.
    req->method = xmalloc(p - buf);

    // strcpy 는 \0(null) 을 만나기 전까지 dst 에 문자열을 복사한다.
    // 즉, 요청라인 첫번째 부터, 위에서 새로 만들어준 \0 까지 복사한다.
    // 그러면 'GET' 이 복사될 것이다.
    strcpy(req->method, buf);

    // 대문자로 변경해준다.
    upcase(req->method);

    // ex) GET /path/to/file HTTP/1.0 에서 '/' 을 path 가 가리킨다.
    path = p;

    // 그 다음 공백을 찾는다. 이 경우 /file 뒤에 ' ' 을 가리키게 될 것이다.
    p = strchr(path, ' ');     

    if (!p) log_exit("parse error on request line (2): %s", buf);

    *p++ = '\0';

    // ex) /path/to/file 할당
    req->path = xmalloc(p - path);

    strcpy(req->path, path);

    if (strncasecmp(p, "HTTP/1.", strlen("HTTP/1.")) != 0)
    {
        log_exit("parse error on request line (3): %s", buf);
    }

    p += strlen("HTTP/1.");     

    // protocol_minor_version : 요청에서 사용된 마이너 버전
    // ex) HTTP 1.1 이면 뒤에 '1' 을 저장한다.
    req->protocol_minor_version = atoi(p);
}

// 헤더 필드 하나씩을 읽어들인다.
// ex) "Data: 2021-09-01"  와 같은 형태로 되어 있다.
static struct HTTPHeaderField* read_header_field(FILE* in)
{
    struct HTTPHeaderField* h;

    char buf[LINE_BUF_SIZE];

    char* p;

    // 일단 한줄을 읽는다.
    if (!fgets(buf, LINE_BUF_SIZE, in))
    {
        log_exit("failed to read request header field: %s", strerror(errno));
    }


    if ((buf[0] == '\n') || (strcmp(buf, "\r\n") == 0))
    {
        // 요청 헤더 필드를 다 읽으면 NULL 을 리턴해줄 것이다.
        // 일반적으로 요청 헤더 필드들을 다 작성한 이후 \r\n 을 써주고
        // 그 다음부터는 요청 본문이 나오게 된다.
        return NULL;
    }

    // buf 에서 ':' 을 찾고, 그 문자의 주소를 p 에 할당한다.
    // ':' 을 기준으로 field 의 이름과 값이 구분되어 있다.
    p = strchr(buf, ':');

    if (!p) log_exit("parse error on request header field: %s", buf);

    // "Data\0 2021-09-01"
    *p++ = '\0';

    h = xmalloc(sizeof(struct HTTPHeaderField));

    h->name = xmalloc(p - buf);

    strcpy(h->name, buf);

    // strspn : '\t' 가 문자열 p 의 앞부분에 몇 글자 나타나는지 세어 그 길이를 반환한다.
    // ex) strspn("ababab--", "ab") -> 6
    // "Data\0 2021-09-01" 에서 p 는 \0 다음의 ' ' 라는 공백 부분을 가리키고 있었다.
    // 그러면 strspn(p, " \t") 는 1을 리턴할 것이다.
    // p += 1 이후에는 '2' 를 가리킬 것이다.
    p += strspn(p, " \t");

    // ex) 2021-09-01  + 끝에 null 문자. 길이만큼의 메모리를 할당한다.
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

/*
* HTTPRequest 와 그 멤버를 모두 해제한다.
*/
static void
free_request(struct HTTPRequest* req)
{
    struct HTTPHeaderField* h, * head;

    // HeaderField 는 연결리스트 형태로 되어 있다.
    // 일일히 돌면서 HeaderField 메모리를 해제해줄 것이다.
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


static long content_length(struct HTTPRequest* req)
{
    char* val;
    long len;

    val = lookup_header_field_value(req, "Content-Length");

    if (!val) return 0;

    // atol : 문자열을 long 으로 변환
    len = atol(val);

    if (len < 0) log_exit("negative Content-Length value");

    return len;
}

static char*
lookup_header_field_value(struct HTTPRequest* req, char* name)
{
    struct HTTPHeaderField* h;

    for (h = req->header; h; h = h->next) {
        // strcasecmp : string 을 비교하기
        if (strcasecmp(h->name, name) == 0)
        {
            return h->value;
        }
    }
    return NULL;
}

/*
네트워크 관련 프로그램에서는 소켓 연결은 어떤 이유로든 갑자기 끊어질 수 있다.
소켓이 끊어지면 xinetd 에서 시작한 경우 SIGPIPE 가 전달된다.
*/
static void install_signal_handlers(void)
{
    trap_signal(SIGPIPE, signal_exit);
}

static void
trap_signal(int sig, sighandler_t handler)
{
    /*
    시그널 : 사용자나 커널이 프로세스에 무언가 통지하는 목적으로 사용된다.
    처리 방법
    - 시그널 무시 
    - 프로세스 종료시키기
    - core 덤프 생성하고 프로세스 비정상 종료 (core dump : 프로세스의 메모리 스냅샷)
    */
    struct sigaction act;

    /*
    시그널 포착 : 시그널이 전달될 때의 동작을 변경하기. 즉, 시그널을 수신할 때 동작을 프로세스가 변경할 수 있다.
    */
    act.sa_handler = handler;           // 시그널 핸들러 설정

    sigemptyset(&act.sa_mask);	    // sa_mask 는 비워두기

    act.sa_flags = SA_RESTART;		// 시스템 콜을 재가동 ?

    if (sigaction(sig, &act, NULL) < 0)
    {
        log_exit("sigaction() failed: %s", strerror(errno));
    }
}

// 프로세스가 시그널을 포착할 때 실행하는 함수
static void signal_exit(int sig)
{
    log_exit("exit by signal %d", sig);
}


static void* xmalloc(size_t sz)
{
    void* p;

    p = malloc(sz);
    if (!p) log_exit("failed to allocate memory");
    return p;
}

static void log_exit(char* fmt, ...)
{
    va_list ap;

    // va_list 변수에 가변인자가 모두 들어가게 된다.
    va_start(ap, fmt);
    
    // vfprintf : 인자가 va_list 로 되어 있는 fprintf 함수
    vfprintf(stderr, fmt, ap);
    
    fputc('\n', stderr);
    va_end(ap);
    exit(1);
}