#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char *path);

/*경로에 있는 파일명들을 표시해주는 기능*/
int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        do_ls(argv[i]);
    }
    exit(0);
}

static void
do_ls(char *path)
{
    DIR *d;
    struct dirent *ent;

    
    d = opendir(path);          /* (1) 경로에 있는 디렉터리를 연다 */
    if (!d) {
        perror(path);
        exit(1);
    }
    while (ent = readdir(d)) {  /* (2) 더 읽을 엔트리가 없을 때까지, null이 아닐 때까지 반복 */
        printf("%s\n", ent->d_name);
    }
    closedir(d);                /* (1') opendir 이후 반드시 close 해줘야 한다. */
}