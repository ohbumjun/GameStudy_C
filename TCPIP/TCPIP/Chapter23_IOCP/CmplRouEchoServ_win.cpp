// #include <stdio.h>
// #include <stdlib.h>
// #include <WinSock2.h>
// 
// #define BUF_SIZE 1024
// 
// void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
// void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
// void ErrorHandling(const char* message);
// 
// typedef struct
// {
// 	SOCKET hClntScok;
// 	char buf[BUF_SIZE];
// 	WSABUF wsaBuf;
// } PER_IO_DATA, *LPPER_IO_DATA;
// 
// int main(int argc, char* argv[])
// {
// 	WSADATA wsaData;
// 	SOCKET hLisnSock, hRecvSok;
// 	SOCKADDR_IN lisnAdr, recvAdr;
// 
// 	LPWSAOVERLAPPED lpOvlp;
// 	DWORD recvBytes;
// 
// 	LPPER_IO_DATA hbInfo;
// 	int mode = 1, recvAdrSize, flagInfo = 0;
// 
// 	if (argc != 2)
// 	{
// 		printf("Usage : %s <port>\n", argv[0]);
// 		exit(1);
// 	}
// 
// 	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
// 		ErrorHandling("WSAStartUp() error!");
// 
// 	// Overlapped IO �� �����ϴ� Socket ����
// 	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
// 
// 	// hLisnSock �ڵ��� �����ϴ� ������ ����� ��带 �ͺ��ŷ ���� �ϰڴ�.
// 	ioctlsocket(hLisnSock, FIONBIO, (u_long*)&mode);
// 
// 	memset(&lisnAdr, 0, sizeof(lisnAdr));
// 	lisnAdr.sin_family = AF_INET;
// 	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
// 	lisnAdr.sin_port = htons(atoi(argv[1]));
// 
// 	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
// 		ErrorHandling("bind() error!");
// 
// 	if (listen(hLisnSock, 6) == SOCKET_ERROR)
// 		ErrorHandling("listen() error!");
// 
// 	recvAdrSize = sizeof(recvAdr);
// 	
// 	while (1)
// 	{
// 		// �����带 alertable wait ���·� �����.
// 		// ��, �ش� ������ �Ϸ�� IO�� �ִٸ� CP�� �����Ѵ�.
// 		// �ٽ� ���ؼ� CP �� ������ ���� �ش� �Լ��� �ݺ�ȣ���ϴ� ���̴�.
// 		SleepEx(100, TRUE); 
// 
// 		// Ŭ���̾�Ʈ ���� ��û�� ���� ���� ����
// 		// ioctlsocket �Լ� ȣ���� ���� ������ hLisnSock ������ ���
// 		// �ش� accept �Լ� ȣ�� ������ Ŭ���̾�Ʈ ���� ��û�� ���ٸ� INVALID_SOCKET ��ȯ
// 		// WSAGetLastError �� ��� WSAEWOULDBLOCK ��ȯ
// 		hRecvSok = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSize);
// 
// 		if (hRecvSok == INVALID_SOCKET)
// 		{
// 			if (WSAGetLastError() == WSAEWOULDBLOCK)
// 				continue;
// 			else 
// 				ErrorHandling("accept() error!");
// 		}
// 
// 		puts("Client connected...");
// 
// 		/*
// 		�Ź� WSARecv �Լ� ȣ�� ������ ���� �ٸ� overlapped ����ü�� �Ѱ��ֱ� ����
// 		���� �Ҵ�
// 		*/
// 		lpOvlp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
// 		memset(lpOvlp, 0, sizeof(WSAOVERLAPPED));
// 
// 		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
// 		hbInfo->hClntScok = (DWORD)hRecvSok;
// 		(hbInfo->wsaBuf).buf = hbInfo->buf;
// 		(hbInfo->wsaBuf).len = BUF_SIZE;
// 
// 		// CP ������ Event�� �ʿ����
// 		// ���⼭�� Ư�� ����ü�� �����ϴ� �뵵�� ���
// 		lpOvlp->hEvent = (HANDLE)hbInfo;
// 
// 		/*
// 		CP ����� Overlapped IO ����
// 		CR�� ReadCompRouting �Լ� ���
// 		*/
// 		WSARecv(hRecvSok, &(hbInfo->wsaBuf), 1, &recvBytes, (LPDWORD)&flagInfo, 
// 			lpOvlp, ReadCompRoutine);
// 	}
// 
// 	closesocket(hRecvSok);
// 	closesocket(hLisnSock); // ���� ���� �Ҹ�
// 
// 	WSACleanup();
// 
// 	return 0;
// }
// 
// void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, 
// 	LPWSAOVERLAPPED lpOverlapped, DWORD flags)
// {
// 	// �ش� �Լ��� ȣ��Ǿ����� ����, Ŭ���̾�Ʈ�κ��� ������ ������ �Ϸ�Ǿ��ٴ� ���� �ǹ�
// 	// ���� Ŭ���̾�Ʈ���� �ٽ� �� �����͸� �����ؾ� �Ѵ�.
// 
// 	/*
// 	WSARecv �Լ� ȣ��� �����ߴ� LOWSAOVERLAPPED �� ����Ű�� ����ü ������
// 	������ ������ ���⼭ Ȱ���Ѵ�.
// 	*/
// 
// 	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
// 	SOCKET hSock = hbInfo->hClntScok;
// 	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
// 	DWORD sendBytes;
// 
// 	// Ŭ���̾�Ʈ�� EOF ���� (���� ���� ��û)
// 	if (szRecvBytes == 0)
// 	{
// 		closesocket(hSock);
// 		free(lpOverlapped->hEvent);
// 		puts("Client disconnected ...");
// 	}
// 	else
// 	{
// 		bufInfo->len = szRecvBytes;
// 
// 		// CR ����� Overlapped IO ����
// 		// CR�� WriteCompRoutine �Լ��� ����Ѵ�.
// 		// ��, ReadCompRoutine �Լ��� ȣ��Ǹ� WSASend �Լ��� ȣ���ϸ鼭
// 		// �ͺ��ŷ ���� �����Ͱ� �۽ŵǰ� �ϰ�, �۽��� �Ϸ�Ǹ�
// 		// WriteCompRouting �Լ��� ȣ��ǰ� �Ѵ�.
// 		WSASend(hSock, bufInfo, 1, &sendBytes, 0, lpOverlapped, WriteCompRoutine);
// 	}
// 
// }
// 
// void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBytes,
// 	LPWSAOVERLAPPED lpOverlapped, DWORD flags)
// {
// 	// ���� �޼����� ���۵� ���Ŀ� �ش� �Լ��� ȣ��ȴ�
// 	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
// 	SOCKET hSock = hbInfo->hClntScok;
// 	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
// 	DWORD recvBytes;
// 
// 	int flagInfo = 0;
// 
// 	/*
// 	�ٽ� �����͸� �����ؾ� �ϹǷ�
// 	CR ����� Overlapped IO ����
// 	CR �� ReadCompRouting �Լ� ���
// 	
// 	��, WriteCompRoutine �Լ��� ȣ��Ǹ� �ٽ� WSARecv�Լ��� ȣ���ϸ鼭
// 	�ͺ��ŷ ���� �������� ������ ��ٸ��� (�ش� ���� �ݺ�)
// 	*/
// 	WSARecv(hSock, &(hbInfo->wsaBuf), 1, &recvBytes, (LPDWORD)&flagInfo,
// 		lpOverlapped, ReadCompRoutine);
// }
// 
// void ErrorHandling(const char* message)
// {
// 
// }
