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
		ǥ�� ����� �Լ� : "����"
		- ������ �����ϸ� �⺻������ �ü���� ���ؼ� ����� ���۰� �����ȴ�.
		��. ǥ�� ����� �Լ��� ����ϸ�, ���Ҿ� �߰��� �� �ϳ��� ���۸� �����Ѵ�.

		��ǻ�� -> ����� �Լ� ����(�ü�� default) -> ǥ�� ����� �Լ� ����(�۽�) -> ���ͳ�
		��ǻ�� <-����� �Լ� ����(�ü�� default) <- ǥ�� ����� �Լ� ����(����) <- ���ͳ�

		>> ����� �Լ� ���� �뵵
		- TCP ������ ���� ������ ���ϴ�
		- TCP �� ��� ������ �н��� �Ǹ� �������� �Ѵ�.
		�׷��� �������� �̷����ٴ� ���� �����͸� ��򰡿� �����س��Ҵٴ� �ǹ̰� �ȴ�.
		��� �����ϴ°� ?
		������ ��� �����̴�.

		>> ǥ�� ����� �Լ� ���� �뵵
		- ������ ���� ����� �������� ������ �ȴ�.

		����, ��� ��Ȳ���� ����� ������ ���������� �ʴ´�
		1) �����ϴ� ������ ��
		2) ��� ���۷��� ������ �̵�Ƚ��

		ex) 1 ����Ʈ ¥�� �����͸� �� 10���� ���ļ� (�� ���� ��Ŷ) ���ļ� ������ ���
		vs
		���۸��ؼ� 10����Ʈ�� �ѹ��� �����ϴ� ��Ȳ

		������ ������ ���� ������ ��Ŷ���� ��� ������ �߰��ȴ�.
		�̴� ������ ũ�⿡ ������� ������ ũ�� ������ ���´�.

		��Ŷ�� 40����Ʈ�� ��Ƶ�, ������ ���� 
		1����Ʈ 10ȸ = 400 ����Ʈ. 10����Ʈ 1ȸ = 40����Ʈ

		��, ������ �翡 ���̰� ����.

		ex) ���� ������ ������ ���� ������ ��� ���۷� �����͸� �̵���Ű�� ����
		�ð��� ���̼Ҹ� �ȴ�.

		1 ����Ʋ 10ȸ �̵���Ű�� �ð���, �̸� ��� 10����Ʈ�� �ѹ�
		�̵���Ű�� �ͺ��� 10�� �� ����� �ð��� �Ҹ�ȴ�.
		*/

		/*
		* fdopen: ���� ��ũ���͸� FILE ����ü �����ͷ� ��ȯ���ִ� �Լ�
		*/
		readfp = fdopen(clnt_sock, "r");
		writefp = fdopen(clnt_sock, "w");

		/*
		ǥ�� ����� �Լ� ��뿡 �־� ������ ����
		1) ����� ����� ���� �ʴ� (?)
		2) ��Ȳ�� ���� fflush �Լ� ȣ���� ����� �����Ѵ�.
		3) ���� ��ũ���͸� FILE ����ü �����ͷ� ��ȯ�ؾ� �Ѵ�.

		������ �� ��, �а� ���Ⱑ ���ÿ� �����Ϸ���
		r+, w+, a+ ���� ����� �Ѵ�.

		�̰��� ���ΰ� �ƴϴ�.
		���۸� ������ ���ؼ� �б⿡�� �����
		���⿡�� �б�� �۾��� ���¸� ������ ������ fflush �Լ��� ȣ���ؾ� �Ѵ�.

		�̷��� �Ǹ� ǥ�� ����� �Լ��� ������ ���۸� ����� ���� ��󿡵�
		������ ��ģ��.

		���� ) fflush �Լ��� �ַ� ��� ��Ʈ���� ���� ���˴ϴ�. 
				�� �Լ��� ������ ��� ���۸� ����(�Ǵ� �÷���) ���ۿ� �ִ� �����͸� 
				���� ��ũ �����̳� �͹̳ο� ����ϴ� ���Դϴ�.
				fflush �Լ��� �ַ� ����� ��� �͹̳ο� ǥ���ϱ� ���� ���˴ϴ�. 
				���� ���, �߿��� ������ ����ϰ� ���۸� ��� ����� �� �� fflush �Լ��� ����� �� �ֽ��ϴ�.

		�Ӹ� �ƴ϶� ǥ�� ����� �Լ��� ����Ϸ��� FILE ����ü��
		������(FILE) �� ��ȯ�ؾ� �Ѵ�.
		��, ������ �����ÿ� ���� ��ũ���͸� ��ȯ�ϴµ�
		�̸� FILE ����ü �����ͷ� ��ȯ�ϴ� ��뵵 �߻��Ѵ�.
		*/
		while (!feof(readfp))
		{
			// �Ʒ� 3���� ���� ���񽺸� �������ִ� �κ��̴�.
			fgets(message, BUF_SIZE, readfp);
			fputs(message, writefp);
			// fflush �Լ��� ����ؾ�, Ŭ���̾�Ʈ�� �����Ͱ� ���۵ȴٰ� �� �� �ִ�.
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