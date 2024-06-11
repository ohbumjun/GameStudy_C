#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// tcp 접속 담당 
// host     : host 명-> localhost (프로세스가 동작하고 있는 호스트) 사용
// service : 포트 번호. 포트번호를 문자열로 전달하는 것도 가능하다.
//              well port ex) 13
static int open_connection(const char* host, char* service);

int
main(int argc, char* argv[])
{
    int sock;
    FILE* f;
    char buf[1024];

    // 해당 함수를 통해 스트림을 얻을 수 있다.
    sock = open_connection(argc > 1 ? argv[1] : "localhost", "daytime");

    f = fdopen(sock, "r");

    if (!f)
    {
        perror("fdopen(3)");
        exit(1);
    }

    // fgets 함수를 통해 한줄씩 읽는다.
    fgets(buf, sizeof buf, f);

    fclose(f);

    fputs(buf, stdout);

    exit(0);
}

static int
open_connection(const char* host, char* service)
{
    int sock;
    struct addrinfo hints, * res, * ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));

    // AF_UNSPEC : 어드레스 패밀리를 지정하지 않는 것.
    // IPv4, IPv6 어느 것중 하나를 사용해도 된다는 의미.
    hints.ai_family = AF_UNSPEC;

    // TCP
    // 패킷이 아니라, 스트림의 접속을 사용한다는 의미.
    hints.ai_socktype = SOCK_STREAM;

    // hints 는 반환하는 ip 주소의 후보를 줄이기 위해 지정한다.
    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }

    // getaddrinfo 을 통해 얻은 값이 반환값이 연결리스트
    // 따라서 연결리스트를 순회한다.
    // 순회하면서 처음으로 접속이 되는 주소를 사용하도록 한다.
    for (ai = res; ai; ai = ai->ai_next) {

        // 소켓을 만들고. 이에 대응되는 파일 디스크립터를 반환한다.
        // 1st : domain /2nd : socket type/ 3rd : protocol 
        // 이 3가지를 모두 합쳐서 소켓에 무엇을 연결할지 지정한다.
        // ex) IPv4 TCP : PF_INET, SOCK_STREAM, 0
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

        // 성공시 0이상의 정수를 반환. 실패시 -1을 반환
        if (sock < 0) {
            continue;
        }

        // 소켓에서 스트림을 꺼내서, addr 로 지정한 주소의 서버에 "스트림" 을 연결한다.
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }

        /* success */
        freeaddrinfo(res);

        return sock;
    }
    fprintf(stderr, "socket(2)/connect(2) failed");
    freeaddrinfo(res);
    exit(1);
}