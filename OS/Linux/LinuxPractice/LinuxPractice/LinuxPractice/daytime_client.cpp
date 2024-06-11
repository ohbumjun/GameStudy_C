#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// tcp ���� ��� 
// host     : host ��-> localhost (���μ����� �����ϰ� �ִ� ȣ��Ʈ) ���
// service : ��Ʈ ��ȣ. ��Ʈ��ȣ�� ���ڿ��� �����ϴ� �͵� �����ϴ�.
//              well port ex) 13
static int open_connection(char* host, char* service);

int
main(int argc, char* argv[])
{
    int sock;
    FILE* f;
    char buf[1024];

    // �ش� �Լ��� ���� ��Ʈ���� ���� �� �ִ�.
    sock = open_connection((argc > 1 ? argv[1] : "localhost"), "daytime");

    f = fdopen(sock, "r");

    if (!f) 
    {
        perror("fdopen(3)");
        exit(1);
    }

    // fgets �Լ��� ���� ���پ� �д´�.
    fgets(buf, sizeof buf, f);

    fclose(f);

    fputs(buf, stdout);

    exit(0);
}

static int
open_connection(char* host, char* service)
{
    int sock;
    struct addrinfo hints, * res, * ai;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));

    // AF_UNSPEC : ��巹�� �йи��� �������� �ʴ� ��.
    // IPv4, IPv6 ��� ���� �ϳ��� ����ص� �ȴٴ� �ǹ�.
    hints.ai_family = AF_UNSPEC;

    // TCP
    // ��Ŷ�� �ƴ϶�, ��Ʈ���� ������ ����Ѵٴ� �ǹ�.
    hints.ai_socktype = SOCK_STREAM;

    // hints �� ��ȯ�ϴ� ip �ּ��� �ĺ��� ���̱� ���� �����Ѵ�.
    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }

    // getaddrinfo �� ���� ���� ���� ��ȯ���� ���Ḯ��Ʈ
    // ���� ���Ḯ��Ʈ�� ��ȸ�Ѵ�.
    // ��ȸ�ϸ鼭 ó������ ������ �Ǵ� �ּҸ� ����ϵ��� �Ѵ�.
    for (ai = res; ai; ai = ai->ai_next) {

        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

        if (sock < 0) {
            continue;
        }

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