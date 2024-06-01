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

    // ���α׷� ���� 1�� : ���� ��Ʈ�� ���
    install_signal_handlers();

    // ǥ�� �Է°� ǥ�� ����� ����
    service(stdin, stdout, argv[1]);

    exit(0);
}

static void
service(FILE* in, FILE* out, char* docroot)
{
    struct HTTPRequest* req;

    // in ��Ʈ������ HTTP ��û�� �а� �� ������ req �� ����
    req = read_request(in);

    // req �� ���� ������ out ��Ʈ���� �� �ִ´�.
    // �̶� �����ϴ� ���� ��Ʈ�� docroot �̴�.
    respond_to(req, out, docroot);

    // service �� �ѹ��� ȣ��. ��, ���� http ������ �ϳ��� ��û�� �޾Ƽ� ó���ϰ� ����
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
     
    ��, HTTPHeaderField �� ���Ḯ��Ʈ. ���·� ����Ǿ� �ִ�.
    HTTPHeaderField �ϳ���, ��� �ʵ� �Ѱ�(�̸�, ���� ��Ʈ) �� ��Ÿ����.
    */
    struct HTTPHeaderField* h;

    req = xmalloc(sizeof(struct HTTPRequest));

    // HTTP ��û�� ù��° �ٿ� �ش��ϴ� "��û ����" �� �д´�.
    // ex) GET/path/to/file HTTP/1.1
    read_request_line(req, in);

    req->header = NULL;                     // header : HTTP ���

    // �̾ ��û ����� �ϳ��� �о���� ���̴�.
    // �׸��� �� ������ HTTPHeaderField �� ���� ���Ḯ��Ʈ�� �߰��Ѵ�.
    while (h = read_header_field(in)) {
        // ���Ḯ��Ʈ�� �ڿ��� �� ����? ���� ������ �ذ���.
        h->next = req->header;
        req->header = h;
    }

    // ��û ��� �κ��� ��� ������
    // ���Ŀ��� body �κ��� �о���� ���̴�.
    // HTTP ��û�� ��ƼƼ body �� ������ ���,
    // Ŭ���̾�Ʈ�� ��ƼƼ Body �� ���̸� Content-Length �ʵ忡 ���� �Ǿ� �־, �� ������ �����Ѵ�.
    req->length = content_length(req);

    if (req->length != 0) 
    {
        if (req->length > MAX_REQUEST_BODY_LENGTH)
        {
            log_exit("request body too long");
        }

        // body : char* == ���� char �� ����Ű�� ������
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

// HTTP ��û�� ù��° �ٿ� �ش��ϴ� "��û ����" �� �д´�.
// ex) GET /path/to/file HTTP/1.0
static void read_request_line(struct HTTPRequest* req, FILE* in)
{
    char buf[LINE_BUF_SIZE];

    char* path, * p;

    // �ϴ� ������ �д´�.
    // ��Ʈ��ũ�� ���� ���۵Ǵ� �����ʹ� �ŷ��� �� �����Ƿ� ������ �־� �Ѵ�.
    // �׷��� ���� �ִ� ���̸� LINE_BUF_SIZE �� �����Ѵ�.
    // buf �� ��û ���� char �� ù��° ���ڿ� �ּҸ� ����ų ���̴�.
    if (!fgets(buf, LINE_BUF_SIZE, in))
    {
        log_exit("no request line");
    }

    // Searches for the first occurrence of the space character (' ') in the string `buf` and assigns its address to `p`
    // ��, �� ��� ���� �κ��� ã�´�.
    p = strchr(buf, ' ');     

    if (!p) log_exit("parse error on request line (1): %s", buf);

    // ���� ����Ű�� ��ġ�� '\0' �� �����ϰ� ������ ��ĭ �����Ѵ�.
    // ���� p �� GET ���� ' ' ��� ������ ����Ű�� �־���. �ش� ' ' �� \0  ���� �ٲٰ� p �� ��ĭ �̵��Ѵ�.
    // ��, �� ���� '/' ���� �̵��Ѵٰ� �����ϸ� �ȴ�.
    *p++ = '\0';

    // method : ��û �޼ҵ�(GET, HEAD, POST) ���� ��� "�빮��" �� �����Ѵ�.
    req->method = xmalloc(p - buf);

    // strcpy �� \0(null) �� ������ ������ dst �� ���ڿ��� �����Ѵ�.
    // ��, ��û���� ù��° ����, ������ ���� ������� \0 ���� �����Ѵ�.
    // �׷��� 'GET' �� ����� ���̴�.
    strcpy(req->method, buf);

    // �빮�ڷ� �������ش�.
    upcase(req->method);

    // ex) GET /path/to/file HTTP/1.0 ���� '/' �� path �� ����Ų��.
    path = p;

    // �� ���� ������ ã�´�. �� ��� /file �ڿ� ' ' �� ����Ű�� �� ���̴�.
    p = strchr(path, ' ');     

    if (!p) log_exit("parse error on request line (2): %s", buf);

    *p++ = '\0';

    // ex) /path/to/file �Ҵ�
    req->path = xmalloc(p - path);

    strcpy(req->path, path);

    if (strncasecmp(p, "HTTP/1.", strlen("HTTP/1.")) != 0)
    {
        log_exit("parse error on request line (3): %s", buf);
    }

    p += strlen("HTTP/1.");     

    // protocol_minor_version : ��û���� ���� ���̳� ����
    // ex) HTTP 1.1 �̸� �ڿ� '1' �� �����Ѵ�.
    req->protocol_minor_version = atoi(p);
}

// ��� �ʵ� �ϳ����� �о���δ�.
// ex) "Data: 2021-09-01"  �� ���� ���·� �Ǿ� �ִ�.
static struct HTTPHeaderField* read_header_field(FILE* in)
{
    struct HTTPHeaderField* h;

    char buf[LINE_BUF_SIZE];

    char* p;

    // �ϴ� ������ �д´�.
    if (!fgets(buf, LINE_BUF_SIZE, in))
    {
        log_exit("failed to read request header field: %s", strerror(errno));
    }


    if ((buf[0] == '\n') || (strcmp(buf, "\r\n") == 0))
    {
        // ��û ��� �ʵ带 �� ������ NULL �� �������� ���̴�.
        // �Ϲ������� ��û ��� �ʵ���� �� �ۼ��� ���� \r\n �� ���ְ�
        // �� �������ʹ� ��û ������ ������ �ȴ�.
        return NULL;
    }

    // buf ���� ':' �� ã��, �� ������ �ּҸ� p �� �Ҵ��Ѵ�.
    // ':' �� �������� field �� �̸��� ���� ���еǾ� �ִ�.
    p = strchr(buf, ':');

    if (!p) log_exit("parse error on request header field: %s", buf);

    // "Data\0 2021-09-01"
    *p++ = '\0';

    h = xmalloc(sizeof(struct HTTPHeaderField));

    h->name = xmalloc(p - buf);

    strcpy(h->name, buf);

    // strspn : '\t' �� ���ڿ� p �� �պκп� �� ���� ��Ÿ������ ���� �� ���̸� ��ȯ�Ѵ�.
    // ex) strspn("ababab--", "ab") -> 6
    // "Data\0 2021-09-01" ���� p �� \0 ������ ' ' ��� ���� �κ��� ����Ű�� �־���.
    // �׷��� strspn(p, " \t") �� 1�� ������ ���̴�.
    // p += 1 ���Ŀ��� '2' �� ����ų ���̴�.
    p += strspn(p, " \t");

    // ex) 2021-09-01  + ���� null ����. ���̸�ŭ�� �޸𸮸� �Ҵ��Ѵ�.
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
* HTTPRequest �� �� ����� ��� �����Ѵ�.
*/
static void
free_request(struct HTTPRequest* req)
{
    struct HTTPHeaderField* h, * head;

    // HeaderField �� ���Ḯ��Ʈ ���·� �Ǿ� �ִ�.
    // ������ ���鼭 HeaderField �޸𸮸� �������� ���̴�.
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

    // atol : ���ڿ��� long ���� ��ȯ
    len = atol(val);

    if (len < 0) log_exit("negative Content-Length value");

    return len;
}

static char*
lookup_header_field_value(struct HTTPRequest* req, char* name)
{
    struct HTTPHeaderField* h;

    for (h = req->header; h; h = h->next) {
        // strcasecmp : string �� ���ϱ�
        if (strcasecmp(h->name, name) == 0)
        {
            return h->value;
        }
    }
    return NULL;
}

// HTTP ��û req �� ���� ������ out �� ����Ѵ�.
static void
respond_to(struct HTTPRequest* req, FILE* out, char* docroot)
{
    if (strcmp(req->method, "GET") == 0)
        do_file_response(req, out, docroot);
    else if (strcmp(req->method, "HEAD") == 0)
        do_file_response(req, out, docroot);
    else if (strcmp(req->method, "POST") == 0)  // 405 Method Not Allowed
        method_not_allowed(req, out);
    else
        not_implemented(req, out);                      // 501 Not Implemented
}

static void
do_file_response(struct HTTPRequest* req, FILE* out, char* docroot)
{
    struct FileInfo* info;

    // ���� ������ ����Ѵ�.
    info = get_fileinfo(docroot, req->path);

    if (!info->ok) {
        free_fileinfo(info);
        not_found(req, out);
        return;
    }
    
    // ���� ������ ���� ���� ����� ���� ������ ����Ѵ�.
    output_common_header_fields(req, out, "200 OK");

    fprintf(out, "Content-Length: %ld\r\n", info->size);                    // ���� ���� ����

    fprintf(out, "Content-Type: %s\r\n", guess_content_type(info)); // ���� ���� ������ ���� ex) text/html, text/plain

    fprintf(out, "\r\n");

    if (strcmp(req->method, "HEAD") != 0) 
    {
        int fd;
        char buf[BLOCK_BUF_SIZE];
        ssize_t n;

        fd = open(info->path, O_RDONLY);

        if (fd < 0)
        {
            log_exit("failed to open %s: %s", info->path, strerror(errno));
        }

        for (;;) 
        {
            n = read(fd, buf, BLOCK_BUF_SIZE);

            if (n < 0)
            {
                log_exit("failed to read %s: %s", info->path, strerror(errno));
            }

            if (n == 0)
            {
                break;
            }

            if (fwrite(buf, 1, n, out) < n)
            {
                log_exit("failed to write to socket");
            }
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

    // �ð� ���� API
    t = time(NULL);
    tm = gmtime(&t);

    if (!tm)
    {
        log_exit("gmtime() failed: %s", strerror(errno));
    }

    /*
    - �ð��� ���ڿ��� ���
    
    (����) �������� ����� ���, strftime �� ������ �� �� �ִ�.
    �����Ͽ� ������� Ư�� �������� �ð��� ����ؾ� �ϴµ�
    setlocale �� ���¿��� strftime �� ����ϸ� ���̺귯���� �����Ͽ� ���缭 ����Ѵ�.
    ������ setlocal �� ���߱� ������ ������ �ȵȴ�.
    ������ locale �� ����ؾ� �Ѵٸ�, ���� struct tm ����� ����ؾ� �Ѵ�.
    */
    strftime(buf, TIME_BUF_SIZE, "%a, %d %b %Y %H:%M:%S GMT", tm);

    fprintf(out, "HTTP/1.%d %s\r\n", HTTP_MINOR_VERSION, status);
    fprintf(out, "Date: %s\r\n", buf);                                                              // ���� ��ȯ�� ��¥�� �ð�
    fprintf(out, "Server: %s/%s\r\n", SERVER_NAME, SERVER_VERSION);        // HTTP ������� ����
    fprintf(out, "Connection: close\r\n");														 // ���� �۽� ��, ��Ʈ���� ������ ����
}

/*
FileInfo
- char* path;   // ���� �ý��� ������ ���� ���
- long size;      // ���� ũ��(����Ʈ)
- int ok;           // ���� �����ϸ� 0�� �ƴѰ�
*/
static struct FileInfo*
get_fileinfo(char* docroot, char* urlpath)
{
    // docroot : ���� ��Ʈ
    // urlpath : URL ���
    /*
    HTTP ������ ���� �ý����� �����ϴ�
    URL �� ���� �ý��ۿ����� ���.

    http://www.example.com/foo/bar.html �� �������ϸ�
    www.example.com ������ /foo/bar.html ���Ͽ� �������ϴ� �Ͱ� ����.
    */
    struct FileInfo* info;
    struct stat st;

    info = xmalloc(sizeof(struct FileInfo));

    // build_fspath �� ���� URL ��û�� ���� ���� �ý����� ��θ� ����
    info->path = build_fspath(docroot, urlpath);
    info->ok = 0;

    /*
    >> stat �� lstat �� ���� Ȥ�� ��ο� ���� ������ ��ȸ�ϴ� �Լ��̴�.

    >> lstat �� ���� ���� ���� Ȯ��
    - ��Ʈ�� �����ϴ°�
    - �װ��� ���� �����ΰ� 
    �ϳ��� �������� ������ fileInfo �� ok ����� 0 ���� �Ѵ�.

    >> lstat �� ����ϴ� ������ ����.
    stat �Լ���, ���� Ʈ���� �ܺθ� ����Ű�� �ɺ��� ��ũ�� ������ ������ �߻��Ѵ�.
    �ϴ� ���⼭�� ��� �ɺ��� ��ũ�� ������ ���̴�.
    */

    if (lstat(info->path, &st) < 0) return info;

    /*
    Unix System ������ file �� type �� st_mode ��� ������ ���� ǥ���ȴ�.
    ���� �ٸ� type �� ���� ? �� �ǹ��ϴ� bit ���� �������� �Ǿ� �ִ�.

    S_ISREG : �� bitwise operation �� �����Ѵ�.
    - ���� �Ϲ� �����̶�� true �� ����
    - �Ϲ� ������ �ƴ϶�� ex) directory, symbolic link ���
    */
    if (!S_ISREG(st.st_mode)) return info;

    info->ok = 1;
    info->size = st.st_size;

    return info;
}

static char*
build_fspath(char* docroot, char* urlpath)
{
    char* path;

    // ���� ��θ� ������ش�.
    // +1 �� 2�� ���ִ� ����.  docroot �� urlpath ������  '/' ��
    // �������� '0' �� �߰��ϱ� ����
    path = xmalloc(strlen(docroot) + 1 + strlen(urlpath) + 1);

    // sprintf : ���ڿ� �����Լ�
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

/*
��Ʈ��ũ ���� ���α׷������� ���� ������ � �����ε� ���ڱ� ������ �� �ִ�.
������ �������� xinetd ���� ������ ��� SIGPIPE �� ���޵ȴ�.
*/
static void install_signal_handlers(void)
{
    trap_signal(SIGPIPE, signal_exit);
}

static void
trap_signal(int sig, sighandler_t handler)
{
    /*
    �ñ׳� : ����ڳ� Ŀ���� ���μ����� ���� �����ϴ� �������� ���ȴ�.
    ó�� ���
    - �ñ׳� ���� 
    - ���μ��� �����Ű��
    - core ���� �����ϰ� ���μ��� ������ ���� (core dump : ���μ����� �޸� ������)
    */
    struct sigaction act;

    /*
    �ñ׳� ���� : �ñ׳��� ���޵� ���� ������ �����ϱ�. ��, �ñ׳��� ������ �� ������ ���μ����� ������ �� �ִ�.
    */
    act.sa_handler = handler;           // �ñ׳� �ڵ鷯 ����

    sigemptyset(&act.sa_mask);	    // sa_mask �� ����α�

    act.sa_flags = SA_RESTART;		// �ý��� ���� �簡�� ?

    if (sigaction(sig, &act, NULL) < 0)
    {
        log_exit("sigaction() failed: %s", strerror(errno));
    }
}

// ���μ����� �ñ׳��� ������ �� �����ϴ� �Լ�
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

    // va_list ������ �������ڰ� ��� ���� �ȴ�.
    va_start(ap, fmt);
    
    // vfprintf : ���ڰ� va_list �� �Ǿ� �ִ� fprintf �Լ�
    vfprintf(stderr, fmt, ap);
    
    fputc('\n', stderr);
    va_end(ap);
    exit(1);
}