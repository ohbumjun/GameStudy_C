#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

/*
// WSAStartUp ���� : 1) ���α׷����� �䱸�ϴ� ������ ������ ������ �˸���
//								2) �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾� ����
// wVersionRequested : ���α׷��Ӱ� ����� ������ ���� ���� ����
// ���� 8��Ʈ   : �� ���� ����, ���� 8��Ʈ : �� ���� ����
// lpWSAData : WSADATA ����ü ������ �ּҰ� ����
int WSAStartUp(WORD wVersionRequested, LPWSADATA lpWSAData);

// ���� ���̺귯�� ���� 
int WSACleanup(void);

// ������ ���� �ڵ� ��ȯ
SOCKET socket(int af, int type, int protocol);

// IP �ּ�, PORT ��ȣ �Ҵ� ���� (��, ���Ͽ� �ּ� �Ҵ�)
int bind(SOCKET s, const struct sockaddr* name, int namelen);

// ���� ��û ���� ���·� �����
int listen(SOCKET s, int backlog);

// Ŭ���̾�Ʈ ���α׷������� ���� ��û ����
SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen);

// Ŭ���̾�Ʈ���� ���� ��� ���� ��û�� ȣ���ϴ� �Լ�
int connect(SOCKET s, const struct sockaddr* name, int namelen);

// ���� ���� �� 
int closesocket(SOCKET s);
*/

/* ������ ��� ����� �Լ�
int send(SOCKET s, const char* buf, int len, int flags);
	s : ������ ���� ��� ���� �ڵ�
	buf : ���� ������
	len : ������ ����Ʈ ��
	flags : �ɼ�

int recv(SOCKET s, const char* buf, int len, int flags);
	s : ������ ���� ��� ���� �ڵ�
	buf : ���� ������
	len : ������ ����Ʈ ��
	flags : �ɼ�
*/