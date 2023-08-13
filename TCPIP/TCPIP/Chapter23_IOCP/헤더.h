#pragma once

/*
> CP ������Ʈ ����

HANDLE WINAPI CreateIoCompletionPort(
  _In_     HANDLE    FileHandle,    // CP ������Ʈ ������ INVALID_HANDLE_VALUE ����
  _In_opt_ HANDLE    ExistingCompletionPort, // CP ������Ʈ ������ NULL ����
  _In_     ULONG_PTR CompletionKey, // CP ������Ʈ ������ 0 ����   
  _In_     DWORD     NumberOfConcurrentThreads // CP ������Ʈ�� �Ҵ�Ǿ� �Ϸ��
                                               // IO�� ó���� ������ �� ����
                                               // ex) 2�� �����ϸ� CP ������Ʈ��
                                               // �Ҵ�Ǿ� ���� ���� ������ 
                                               // �������� ���� 2���� ����
                                               // 0 �����ϸ�, �ý����� CPU������
                                               // ���� ���� ������ ��������
                                               // �ִ���� ���� (�ھ� ����)
);

> CP ������Ʈ ����

HANDLE WINAPI CreateIoCompletionPort(
  _In_     HANDLE    FileHandle, // CP ������Ʈ�� ������ ���� �ڵ� ����
  _In_opt_ HANDLE    ExistingCompletionPort, // ���ϰ� ������ CP ������Ʈ �ڵ� ����
  _In_     ULONG_PTR CompletionKey, // �Ϸ�� IO ���� ������ ������ ���� �Ű�����
  _In_     DWORD     NumberOfConcurrentThreads //  2��° �Ű������� NULL �� �ƴϸ� ����
);

> Completion Port �� �Ϸ�� IO Ȯ�ΰ� �������� IO ó��
- IO�� �����ϴ� �����尡 IO �� �ϷḦ Ȯ���ϱ� ���ؼ� �ش� �Լ��� ȣ��
- ��, ���� IO �� �Ϸῡ ���� ������ �޴� �������� ������ CreateIoCompletionPort ȣ��� ������ �ִ� �������� ���� ���� �ʴ´�.

BOOL GetQueuedCompletionStatus(
  [in]  HANDLE       CompletionPort, // �Ϸ�� IO ������ ��ϵǾ� �ִ� CP ������Ʈ �ڵ� ����

        LPDWORD      lpNumberOfBytesTransferred, // ����� �������� �ۼ��ŵ� ��������
                                                 // ũ�������� ������ ������ �ּҰ� ����

  [out] PULONG_PTR   lpCompletionKey,            // CreateIoCompletionPort �� 3��° ���ڷ� ���޵�
                                                 // ���� ������ ���� ������ �ּҰ� ����

  [out] LPOVERLAPPED *lpOverlapped,              // WSASend, WSARecv �Լ� ȣ��� �����ϴ�
                                                 // OVERLAPPED ����ü ������ �ּҰ��� ����� ���� �ּҰ� ����

  [in]  DWORD        dwMilliseconds              // Ÿ�Ӿƿ� ���� ����, ���⼭ ������ �ð��� �Ϸ�Ǹ� FALSE�� ��ȯ�ϸ鼭 �Լ��� ����������, 
                                                 // INFINITE �� �����ϸ� �Ϸ�� IO�� CP ������Ʈ�� ��ϵ� ������ ���ŷ ���¿� �ְ� �ȴ�.
);

��, GetQueuedCompletionStatus �Լ��� 3��° ���ڸ� ���ؼ� ��� �Ǵ� ���� ���ϰ� CP ������Ʈ�� ������ �������� 
CreateIoCompletionPort �Լ��� ȣ��� �� ���޵Ǵ� 3��° ���ڰ�

GetQueuedCompletionStatus �Լ��� 4��° ���ڷ� ��ԵǴ� ���� WSASend, WSARecv �Լ�ȣ��� ���޵ȴ� WSAOVERLAPPED ����ü ������ �ּҰ�

*/