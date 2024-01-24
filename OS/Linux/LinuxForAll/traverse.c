/*
디렉터리 안에 또 다른 디렉터리가 있을 경우 재귀적으로 순회하여 
파일 목록을 출력하는 프로그램이다.
여기서 주의할 사항은 2가지이다.

1) . 와 ..

. 는 현재 디렉토리
.. 는 상위 디렉토리

만약 .. 를 배제하지 않게 되면
계속해서 상위 루트 디렉토리까지 올라가면서
모든 파일 내용을 계속해서 출력할 것이다.

2) 심볼릭 링크

처리 중인 디렉터리 안에 루트 디렉터리를 가리키는
심볼릭 링크가 있다면 ? 

그러면 루트 디렉터리로 처리가 이동하고
그 안에서 또 루트 디렉터리를 가리키는 심볼릭 링크가 있다면
무한루프에 들어가게 된다.

이러한 문제를 피하기 위해서 lstate() 를 사용하여
심볼릭 링크를 명시적으로 제외해야 한다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

struct strbuf {
    char *ptr;
    size_t len;
};

static void traverse(struct strbuf *buf);
static void traverse0(struct strbuf *buf, int first);

// strbuf 힙 할당 해주는 함수
static struct strbuf *strbuf_new(void);

// size 인자 개수만큼 realloc 해주는 함수
static void strbuf_realloc(struct strbuf *buf, size_t size);

static void print_error(char *s);

static char *program_name;  /* used by print_error() */

int
main(int argc, char *argv[])
{
    struct strbuf *pathbuf;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dir>\n", argv[0]);
        exit(1);
    }
    program_name = argv[0];

    pathbuf = strbuf_new();

    strbuf_realloc(pathbuf, strlen(argv[1]));

    strcpy(pathbuf->ptr, argv[1]);

    traverse(pathbuf);

    exit(0);
}

static void
traverse(struct strbuf *pathbuf)
{
    traverse0(pathbuf, 1);
}

static void
traverse0(struct strbuf *pathbuf, int first)
{

    // first 는 len 길이가 들어오게 된다.
    DIR *d;
    struct dirent *ent;
    struct stat st;

    d = opendir(pathbuf->ptr);
    if (!d) {
        switch (errno) {
        // path is not directory
        case ENOTDIR:   /* might be replaced after last readdir(). */
            return;
        // does not exist..? (no such file or directory)
        case ENOENT:
            if (first) {
                print_error(pathbuf->ptr);
                exit(1);
            }
            else {
                /* might be removed after last readdir(). */
                return;
            }
         /* ignore permission error. */
        case EACCES:   
            puts(pathbuf->ptr);
            print_error(pathbuf->ptr);
            return;
        default:
            print_error(pathbuf->ptr);
            exit(1);
        }
    }
    puts(pathbuf->ptr);

    while (ent = readdir(d)) {
        
        // . 와 .. 배제하기 
        if (strcmp(ent->d_name, ".") == 0) continue;
        if (strcmp(ent->d_name, "..") == 0) continue;
        
        // 디렉토리내의 각 파일 엔트리 이름을 충분히 할당할 수 있게 realloc 을 해준다.
        strbuf_realloc(pathbuf, pathbuf->len + 1 + strlen(ent->d_name) + 1);
        
        /* special handling for the root directory */
        if (strcmp(pathbuf->ptr, "/") != 0) {
            strcat(pathbuf->ptr, "/");
        }
        strcat(pathbuf->ptr, ent->d_name);
        
        // 해당 파일의 정보를 얻어오기 위해 lstat 함수 호출
        if (lstat(pathbuf->ptr, &st) < 0) {
            switch (errno) {
            case ENOENT:    /* might be unlinked after readdir(). */
                break;
            case EACCES:    /* ignore permission error. */
                print_error(pathbuf->ptr);
                break;
            default:
                print_error(pathbuf->ptr);
                exit(1);
            }
        }
        else {
            // symbolic link 인지를 검사한다.
            // symbolic link 가 아니고 + directory 인 경우에만 다시 traverse 하도록 한다.
            if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
                traverse0(pathbuf, 0);
            }
            else {
                puts(pathbuf->ptr);
            }
        }
        *strrchr(pathbuf->ptr, '/') = '\0';
    }

    closedir(d);
}

#define INITLEN 1024

static struct strbuf*
strbuf_new(void)
{
    struct strbuf *buf;
    
    buf = (struct strbuf*)malloc(sizeof(struct strbuf));
    if (!buf) {
        print_error("malloc(3)");
        exit(1);
    }
    buf->ptr = malloc(INITLEN);
    if (!buf->ptr) {
        print_error("malloc(3)");
        exit(1);
    }
    buf->len = INITLEN;
    return buf;
}

static void
strbuf_realloc(struct strbuf *buf, size_t len)
{
    char *tmp;

    if (buf->len > len) return;
    tmp = realloc(buf->ptr, len);
    if (!tmp) {
        print_error("realloc(3)");
        exit(1);
    }
    buf->ptr = tmp;
    buf->len = len;
}

/* imitate find(1)'s error message */
static void
print_error(char *s)
{
    fprintf(stderr, "%s: %s: %s\n", program_name, s, strerror(errno));
}