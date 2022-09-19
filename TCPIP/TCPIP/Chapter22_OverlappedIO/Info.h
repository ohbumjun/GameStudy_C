#pragma once

/*

> Overlapped IO ���� ����                 ---------------------------------------------------------------------

SOCKET WSASocket(
	int af, int type, int protocol, LPWAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);

	- af       : �������� ü��
	- type     : ���� ������ ���۹��
	- protocol : �� ���� ���̿� ���Ǵ� �������� ���� ����
	- lpProtocolInfo : �����Ǵ� ������ Ư�� ������ ��� WSAPROTOCOL_INFO ����ü ������ �ּҰ� ����
	- g : 
	- dwFlags : ������ �Ӽ� ���� ���� --> WSA_FLAG_OVERLAPPED �� ����ϸ� Overlapped IO�� ������ ��-���ŷ ����� ������ �����ȴ�.

> Overlapped IO �� �����ϴ� WSASend �Լ� ---------------------------------------------------------------------

int WSASend(
	SOCKET s,                                                     // ���� �ڵ�
	LPWSABUF lpBuffers,                                           // ������ ������ ������ ���ϴ� WSABUF ����ü ���� �迭 �ּҰ�
	DWORD dwBufferCount,                                          // �ι�° ���� �迭 ���� ����
	LPDWORD lpNumberOfBytesSEnt,                                  // ���۵� ����Ʈ���� ����� ������ �ּҰ�
	DWORD dwFlags,                                                // �Լ��� ������ ����Ư���� �����ϴ� ��쿡 ���ȴ�.
	LPWSAOVERLAPPED lpOverlapped,                                 // WSAOVERLAPPED ����ü ������ �ּҰ� ����
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine        // Completion Routine �̶�� �Լ��� �ּҰ� ���� --> �̸� ���ؼ� ������ ������ �ϷḦ Ȯ���� �� �ִ�.
);

typedef struct __WSABUF
{
	u_long len;    // ������ ������ ũ��
	char FAR* buf; // ������ �ּҰ�
} WSABYF, *LPWSABUF;


typedef struct _WSAOVERLAPPED
{
	DWORD Internal;
	DWORD InternalHigh;
	DWORD Offset;
	DWORD OffsetHigh;  // ������� 4�� ������ �ü�� ���������� ���
	WSAEVENT hEvent;   // ���� ����ϴ� ���� �� ����ġ�̴�.
} WSAOVERLAPPED, *LPWSAOVERLAPPED;

> Overlapped IO �� ���� ���۵� �������� ũ�⸦ Ȯ���ϴ� �Լ� ---------------------------------------------------------------------

BOOL WSAGetOverlappedResult
{ SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags};

- s : Overlapped IO�� ����� ���� �ڵ�
- lpOverlapped : Overlapped IO ���� �� ������ WSAOVERLAPPED ����ü ������ �ּҰ� ����
- lpcbTransfer :���� �ۼ��ŵ� ����Ʈ ũ�⸦ ������ ������ �ּҰ� ����
- fWait : ������ IO�� �������� ��Ȳ�� ��� TRUE ���޽� IO�� �Ϸ�� ������ ���, FALSE ���� �� ��ȯ�ϸ鼭 �Լ��� ����������.
- lpdwFlags : WSARecv �Լ��� ȣ��� ���, �μ����� ������(���ŵ� �޽����� OOB �޽�������)�� ��� ���� ����Ѵ�.���ʿ��ϸ� NULL�� �����Ѵ�.


> Overlapped IO �� �����ϴ� WSARecv �Լ� ---------------------------------------------------------------------

int WSAAPI WSARecv(
  [in]      SOCKET                             s,                    // Overlapped IO �Ӽ��� �ο��� ���� �ڵ�
  [in, out] LPWSABUF                           lpBuffers,            // ���ŵ� ������ ������ ����� ������ ������ ���� WSABUF ����ü �迭�� �ּҰ� ����.
  [in]      DWORD                              dwBufferCount,        // �ι�° ���ڷ� ���޵� �迭�� �������� ���� 
  [out]     LPDWORD                            lpNumberOfBytesRecvd, // ���ŵ� �������� ũ�������� ����� ������ �ּҰ� ����
  [in, out] LPDWORD                            lpFlags,              // ����Ư���� ��õ� ������ �����ϰų� �����ϴ� ���
  [in]      LPWSAOVERLAPPED                    lpOverlapped,         // WSAOVERLAPPED ����ü ������ �ּ� �� ����  
  [in]      LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine   // Completion Routine �̶�� �Լ��� �ּҰ� ����
);


void CALLBACK CompRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);

- ù��° �Ű����� : �������� (���� ����� 0 ����)
- �ι�°   : �Ϸ�� ����� �������� ũ������ ����
- ����°   : WSASend, WSARecv �Լ��� �Ű����� lpOverlapped�� ���޵� �� 
- �׹�°   : ����� �Լ� ȣ��� ���޵� Ư�� ���� �� 0�� ����
- CALLBACK : ������ main �Լ��� ����Ǵ� Ű����� WINAPI�� ���������� �Լ��� ȣ�� �Ծ��� �� ��.
*/