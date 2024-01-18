#include <stdio.h>
#include <stdlib.h>

static void do_head(FILE *f, long nlines);

int
main(int argc, char *argv[])
{
    long nlines;

    fprintf(stdout, "argc=%d\n", argc);
    if (argc < 2) {
        fprintf(stderr, "Usage: %s n [file file...]\n", argv[0]);
        exit(1);
    }
    nlines = atol(argv[1]);
    if (argc == 2) {
        do_head(stdin, nlines);
    } else {    /* else 절이 추가됨 */
        int i;
        // 3번째 인자에는 내가 열고자 하는 파일명이 들어올 것이다.
        // 파일명은 당연히 여러개가 될 수 있다.
        for (i = 2; i < argc; i++) {
            FILE *f;

            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_head(f, nlines);
            fclose(f);
        }
    }
    exit(0);
}

static void
do_head(FILE *f, long nlines)
{
    int c;

    fprintf(stdout, "nlines=%ld\n", nlines);    

    if (nlines <= 0) return;
    while ((c = getc(f)) != EOF) {
        // 표준 입출력에서 바이트 단위 출력
        // 즉, file 스트림에서 읽은 값을 화면에 출력
        if (putchar(c) < 0) exit(1);
        if (c == '\n') {
            nlines--;
            if (nlines == 0) return;
        }
    }
}