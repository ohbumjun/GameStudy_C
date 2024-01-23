#include <stdio.h>
#include <stdlib.h>
/* (1) */
#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

/* 
(2)

긴 옵션을 위한 struct option 의 배열.
각각의 긴 옵션에 해당하는 요소를 정의
배열 마지막에는 모든 멥버를 0 으로 대입한 요소를 넣어야 한다.
*/
static struct option longopts[] = {
    {"lines", required_argument, NULL, 'n'},
    {"help",  no_argument,       NULL, 'h'},
    {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
    int opt;
    long nlines = DEFAULT_N_LINES;

    /* 
    (3) 
    옵션 파싱 루프.


    */
    while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'n':         
        /* 
        (4)
        -n 옵션과 --lines 옵션을 처리한다.
        옵션의 파라미터값은 optarg 에 들어있어
        여기서 출력할 줄 개수를 얻어올 수 있다. 
        */
            nlines = atol(optarg);
            break;
        case 'h':         
        /* 
        (5) 

        --help 옵션을 처리한다.

        */
            fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(0);
        case '?':         
        /* 
        (6)
        
        모르는 옵션을 전달받을 경우
        에러 메세지를 출력한다.


        
        */
            fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(1);
        }
    }
    /* 
    (7) 

    파싱이 끝난 시점에서 optind 가 argc 와 같다는 것은
    옵션 이외에 추가적인 인자를 지정하지 않았다는 것이다

    왜냐하면 optind 는 getopt 나 getopt_long 이 마지막으로 처리한
    인자의 인덱스를 가리키고 있기 때문이다.
    */
    if (optind == argc) {
        do_head(stdin, nlines);
    } else {
        int i;

        /* (7') */
        for (i = optind; i < argc; i++) {
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

    if (nlines <= 0) return;
    while ((c = getc(f)) != EOF) {
        if (putchar(c) < 0) exit(1);
        if (c == '\n') {
            nlines--;
            if (nlines == 0) return;
        }
    }
}