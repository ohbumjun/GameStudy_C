#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

int
main(int argc, char *argv[])
{
    int opt;
    long nlines = DEFAULT_N_LINES;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        /*
        opt 는 옵션 하나하나가 들어오게 된다.
        getopt 함수는, 한번 호출 때마다 다음 옵션으로 넘어가게 되고
        더이상 parsing 할 옵션이 존재하지 않으면 -1 을 리턴하게 된다.
        */
        switch (opt) {
        case 'n':
            nlines = atol(optarg);
            break;
        case '?':
            fprintf(stderr, "Usage: %s [-n LINES] [file...]\n", argv[0]);
            exit(1);
        }
    }
    if (optind == argc) {
        do_head(stdin, nlines);
    } else {
        int i;

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