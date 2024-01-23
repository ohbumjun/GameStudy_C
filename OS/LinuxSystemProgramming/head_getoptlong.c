#include <stdio.h>
#include <stdlib.h>
/* (1) */
#define _GNU_SOURCE // getopt_long 을 사용하기 위해 필요
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

/* (2) 
긴 옵션을 위해 struct option 의 배열을 만든다 
배열 마지막에는 모든 멤버를 0으로 대입한 요소를 넣어야 한다.
*/
static struct option longopts[] = {
    // 각각이 getopt_long 의 struct option 의 멤버에 대응된다.
    {"lines", required_argument, NULL, 'n'},
    {"help",  no_argument,       NULL, 'h'},
    {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
    int opt;
    long nlines = DEFAULT_N_LINES;

    /* (3) 
    옵션을 파싱한다.
    3번째 인자가 hn: 이 아니고 n: 이다.
    
    n : linux 의 옵션 중 하나로 lines 와 동일한 녀석이다
    ':' 라는 것은, 해당 n 이라는 옵션에 인자가 필요하다는 것을 의미한다.
    즉, -n 10 과 같이 인자를 받아야 한다는 것이다.
    */
    while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'n':         /* (4) */
            nlines = atol(optarg);
            break;
        case 'h':         /* (5) */
            fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(0);
        case '?':         /* (6) */
            fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(1);
        }
    }
    /* (7) */
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