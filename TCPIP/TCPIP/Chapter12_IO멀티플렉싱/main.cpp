#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	TIMEVAL timeout;

	// fd_set ���� 
	// ������ �׷����� ���ؼ� ����Ѵ�.
	// ����ü ���ο� �ִ� fd_array �� ����ؼ� ������ ��ȭ��
	// select �Լ��� ���� �����Ѵ�.
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	if (argc != 2)
	{
		printf("Usage %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("WSA StartUp Error\n");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("bind Error\n");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		printf("listen Error\n");

	// ���� ��ũ���� ��� ��Ʈ�� 0���� �ʱ�ȭ
	FD_ZERO(&reads);

	// ���� ������ fd_set �� ������ش�.
	// �ش� ���� ��ũ���͸� 1�� �������ش�.
	// �־��� Ư�� fd �� ��Ʈ�� �Ӷ� ���
	FD_SET(hServSock, &reads); 

	while (1)
	{
		// ������ �����ϱ� ���� copySet �� ����Ѵ�
		// select �Լ��� ȣ���ϸ� ��ȭ�� ���� fd�� ������ ������ ��ġ��
		// ��Ʈ���� ���� 0���� �ʱ�ȭ �ȴ�.
		// ���� ������ �����ؾ� �Ѵ�.
		cpyReads = reads;

		// select �Լ��� �⺻������ fd(Ȥ�� �ڵ�) �� ��ȭ�� ���� ������
		// ����ϴ� ���ŷ������� �Ѱ��� ��Ƴ��� ���� ����
		// ���� ��ũ����(�ڵ�)�� ���ÿ� �����Ѵ�.

		// �б�, ����, ���� ������ �����Ѵ�.
		// int nfds : �����쿡���� ������� �ʴ´�.
		// fd_set* readfds   : ���ŵ� �����Ͱ� �ִ��� Ȯ���� fd
		// fd_set* writefds  : ���� �غ� �� �����Ͱ� �ִ��� Ȯ���� fd
		// fd_set* exceptfds : ���ܹ߻��� �ִ��� Ȯ���� fd
		// const timeval* timeout : Ÿ�� �ƿ� ��Ÿ����.
		// - NULL�� �����ϸ� �ϳ��� ������ ������ ����Ѵ�.
		// - ������̶�� �� �ð����� ���

		int iRet = select(0, &cpyReads, nullptr, nullptr, nullptr);

		if (iRet == SOCKET_ERROR)
			break;

		// ���õ� ���� ����(fd_count)��ŭ �˻��Ѵ�
		for (int i = 0; i < reads.fd_count; ++i)
		{
			// FD_ISSET : ��ȭ ���� Ȯ��
			// ������ Ŀ�Ǽ��� ���Ͽ� ��ȭ ���θ� Ȯ���� �� �ִ�.
			// ��, reads.fd_array[i] �� �༮�� fd �� ��ȭ�� �ִ°�
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				// ��ȭ�� ������ Ŭ�� ������ ��û�� hServSock ������ 
				// ��ȭ�� ����
				if (reads.fd_array[i] == hServSock)
				{
					adrSz = sizeof(clntAddr);

					// ������ �����Ѵ�. �ش� ���� ��û�� ���� ������ �����Ѵ�.
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &adrSz);

					// ���� ������� �߰��Ѵ�(����)
					FD_SET(hClntSock, &reads);

					printf("connected client : %d\n", hClntSock);
				}
				else // read message
				{
					// �ش� ��ȣ ������ ������
					// Ŭ���̾�Ʈ �����̶�� �ǹ��̴�. ��, ���� �Ҹ��� �ƴ� ���
					// ������ �����Ͱ� �ִ��� Ȯ���ؾ� �Ѵ�.
					// ������ �����Ͱ� ���ڿ� ����������, 
					// �ƴϸ�, ���� ���Ḧ �ǹ��ϴ� EOF ���� Ȯ���ؾ� �Ѵ�.
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);

					// EOF --> ���� ���Ḧ �ǹ��Ѵ�.
					if (strLen == 0)
					{
						// ���� ��󿡼� �����Ѵ�.
						FD_CLR(reads.fd_array[i], &reads);

						closesocket(cpyReads.fd_array[i]);

						printf("closed client: %d\n", cpyReads.fd_array[i]);
					}
					else
					{
						// echo
						send(reads.fd_array[i], buf, strLen, 0);
					}
				}
			}
		}
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}