// link 명령어
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    fprintf(stdout, "hello");
    if (argc != 3) {
        fprintf(stderr, "%s: wrong arguments\n", argv[0]);
        exit(1);
    }
    if (link(argv[1], argv[2]) < 0) {
        fprintf(stdout, "fail");
        perror(argv[1]);
        exit(1);
    }
    exit(0);
}