#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

static void make_path(const char *path);
static void die(const char *s);

int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        make_path(argv[i]);
    }
    exit(0);
}

/*
    "Check & Create" strategy may fail in multi-process environment,
    we try to create a directory and check an error instead.
*/
static void
make_path(const char *path)
{
    // mkdir 의 2번째 인자에는 권한을 지정한다.
    // 여기서 지정한 권한과 umask 라는 값 사이의 비트 연산이 이루어진다.
    if (mkdir(path, 0777) == 0) {
        // Successfully created.  OK
        return;
    }
    // path 에 저장한 경로에 이미 파일이나 디렉터리가 존재한다.
    if (errno == EEXIST) {
        struct stat st;
        if (stat(path, &st) < 0) die("stat");
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "file exists but is not a directory: %s\n", path);
            exit(1);
        }
        // Given path is already a directory.
        return;
    }
    // 상위 디렉터리가 없다.
    else if (errno == ENOENT) {
        // Parent path is not a directory.  Make it
        // ex) /usr/local/bin/xxx 를 만들 때 /usr/local/bin 이 없다면 상위 parent 를
        // 만들어가면서 add mkdir 하고자 한다.
        char *parent_path = strdup(path);
        if (!parent_path) die("strdup");

        // parent path 의 끝에서부터 앞쪽 방향으로 순회하면서 '/' 를 지워주는 것이다.
        // ex) parent_path 가 /usr/local/bin/ 일 때 /usr/local/bin 으로 만들어주는 것이다.
        // Removes duplicated trailing slashes ('/')
        char *last = parent_path + strlen(parent_path) - 1;
        while (*last == '/' && last != parent_path) {
            *last-- = '\0';
        }

        // parent_path 가 단순히 '/' 라면 에러 메세지를 출력한다.
        if (strcmp(parent_path, "/") == 0) {
            fprintf(stderr, "error: root directory is not a directory???\n");
            exit(1);
        }

        // parent_path 에서 가장 마지막 '/' 를 찾는다.
        char *sep = strrchr(parent_path, '/');

        if (!sep) {
            // No slash ('/') found.  e.g. "component"
            // 애초에 이상한 경로 정보가 input 으로 들어온 것..
            // ex) path 인자가 component/rendercomponent 이런 식으로 들어온 것
            fprintf(stderr, "error: current directory is not a directory???\n");
            exit(1);
        }
        else if (sep == parent_path) {
            // e.g. "/component"
            // parent_path 중에서 가장 앞에 있는 글자가 '/' 였다면
            fprintf(stderr, "error: root directory is not a directory???\n");
            exit(1);
        }
        // e.g. "/a/b/c" -> "/a/b\0c"
        // 아래 코드를 통해 parent path 를, 그 이전 경로 정보와 분리할 수 있게 된다.
        *sep = '\0';

        // 실제 코드가 어떻게 실행되는지 test 해봐야 할 것 같다.
        make_path(parent_path);

        if (mkdir(path, 0777) < 0) die(path);

        return;
    }
    else if (errno == ENOTDIR) 
    {
        // path 로 지정한 상위디렉터리가 디렉터리가 아니다
        // ex) /usr/src/hello 에서 /usr/src 가 파일인 경우
        perror(path);
        exit(1);
    }
    // 상위 디렉터리에 대한 변경 권한이 없다.
    else if (errno == EPERM) 
    {
        perror(path);
        exit(1);
    }
    else {
        perror(path);
        exit(1);
    }
}

static void
die(const char *s)
{
    perror(s);
    exit(1);
}