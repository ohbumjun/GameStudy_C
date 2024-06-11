#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	FILE* readfp;
	FILE* writefp;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		/*
		표준 입출력 함수 : "버퍼"
		- 소켓을 생성하면 기본적으로 운영체제에 의해서 입출력 버퍼가 생성된다.
		자. 표준 입출력 함수를 사용하면, 더불어 추가로 또 하나의 버퍼를 제공한다.

		컴퓨터 -> 입출력 함수 버퍼(운영체제 default) -> 표준 입출력 함수 버퍼(송신) -> 인터넷
		컴퓨터 <-입출력 함수 버퍼(운영체제 default) <- 표준 입출력 함수 버퍼(수신) <- 인터넷

		>> 입출력 함수 버퍼 용도
		- TCP 구현을 위한 목적이 강하다
		- TCP 의 경우 데이터 분실이 되면 재전송을 한다.
		그런데 재전송이 이뤄진다는 것은 데이터를 어딘가에 저장해놓았다는 의미가 된다.
		어디에 저장하는가 ?
		소켓의 출력 버퍼이다.

		>> 표준 입출력 함수 버퍼 용도
		- 오로지 성능 향상을 목적으로 제공이 된다.

		물론, 모든 상황에서 우월한 성능을 보여주지는 않는다
		1) 전송하는 데이터 양
		2) 출력 버퍼로의 데이터 이동횟수

		ex) 1 바이트 짜리 데이터를 총 10번에 걸쳐서 (열 개의 패킷) 걸쳐서 보내는 경우
		vs
		버퍼링해서 10바이트로 한번에 전송하는 상황

		데이터 전송을 위해 구성된 패킷에는 헤더 정보가 추가된다.
		이는 데이터 크기에 상관없이 일정한 크기 구조를 갖는다.

		패킷당 40바이트만 잡아도, 데이터 양이 
		1바이트 10회 = 400 바이트. 10바이트 1회 = 40바이트

		즉, 데이터 양에 차이가 난다.

		ex) 또한 데이터 전송을 위해 소켓의 출력 버퍼로 데이터를 이동시키는 데도
		시간이 많이소모 된다.

		1 바이틀 10회 이동시키는 시간이, 이를 묶어서 10바이트를 한번
		이동시키는 것보다 10배 더 가까운 시간이 소모된다.
		*/

		/*
		* fdopen: 파일 디스크립터를 FILE 구조체 포인터로 변환해주는 함수
		*/
		readfp = fdopen(clnt_sock, "r");
		writefp = fdopen(clnt_sock, "w");

		/*
		표준 입출력 함수 사용에 있어 불편한 사항
		1) 양방향 통신이 쉽지 않다 (?)
		2) 상황에 따라 fflush 함수 호출이 빈번히 등장한다.
		3) 파일 디스크립터를 FILE 구조체 포인터로 변환해야 한다.

		파일을 열 때, 읽고 쓰기가 동시에 가능하려면
		r+, w+, a+ 모드로 열어야 한다.

		이것이 전부가 아니다.
		버퍼링 문제로 인해서 읽기에서 쓰기로
		쓰기에서 읽기로 작업의 형태를 변경할 때마다 fflush 함수를 호출해야 한다.

		이렇게 되면 표준 입출력 함수의 장점인 버퍼링 기반의 성능 향상에도
		영향을 미친다.

		참고 ) fflush 함수는 주로 출력 스트림을 위해 사용됩니다. 
				이 함수의 목적은 출력 버퍼를 비우고(또는 플러시) 버퍼에 있는 데이터를 
				실제 디스크 파일이나 터미널에 출력하는 것입니다.
				fflush 함수는 주로 출력을 즉시 터미널에 표시하기 위해 사용됩니다. 
				예를 들어, 중요한 정보를 출력하고 버퍼를 즉시 비워야 할 때 fflush 함수를 사용할 수 있습니다.

		뿐만 아니라 표준 입출력 함수를 사용하려면 FILE 구조체의
		포인터(FILE) 로 변환해야 한다.
		즉, 소켓은 생성시에 파일 디스크립터를 변환하는데
		이를 FILE 구조체 포인터로 변환하는 비용도 발생한다.
		*/
		while (!feof(readfp))
		{
			// 아래 3줄은 에코 서비스를 제공해주는 부분이다.
			fgets(message, BUF_SIZE, readfp);
			fputs(message, writefp);
			// fflush 함수를 사용해야, 클라이언트로 데이터가 전송된다고 할 수 있다.
			fflush(writefp);
		}
		fclose(readfp);
		fclose(writefp);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}