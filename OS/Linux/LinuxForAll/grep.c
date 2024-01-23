#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *f);

int
main(int argc, char *argv[])
{
    regex_t pat;
    int err;
    int i;

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    // 정규표현식의 패턴 문자열을 regex_t 로 변환한다.
    // 정상적으로 변환이 되면 0 을 출력할 것이다.
    err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
    
    if (err != 0) {
        char buf[1024];

        // regerror : 에러가 발생하면, 해당 에러를 출력하는 함수
        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (argc == 2) {
        do_grep(&pat, stdin);
    }
    else {
        for (i = 2; i < argc; i++) {
            FILE *f;
            
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }

    // regcomp 가 open 이라면, regfree 는 free 이다. (리소스를 해제하는 함수)
    // 둘은 한 세트이다.
    regfree(&pat);
    exit(0);
}

static void
do_grep(regex_t *pat, FILE *src)
{
    char buf[4096];

    // 전달받은 파일 스트림의 문자열을 "한줄" 단위로 읽어들인다.
    while (fgets(buf, sizeof buf, src)) {
        // 읽어들인 문자열이 내가 원하는 정규 표현식 패턴에 부합하는지 살핀다.
        // regexec 는 문자열와 패턴을 조합하는 API 로 , 문자열 string 이 패턴에
        // 부합할 경우 0 을 반환한다.
        if (regexec(pat, buf, 0, NULL, 0) == 0) {
            // 표준 출력으로 해당 줄을 출력한다.
            fputs(buf, stdout);
        }
    }
}