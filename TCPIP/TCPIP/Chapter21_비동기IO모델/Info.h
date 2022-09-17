#pragma once

/*

int WSAEventSelect(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);

	- ������ 0, ���н� SOCKET_ERROR ��ȯ
	- s : ���� ��� ������ �ڵ�
	- hEventObject  : �̺�Ʈ �߻� ������ Ȯ���� ���� Event ������Ʈ�� �ڵ� ����
	- INetworkEvent : �����ϰ��� �ϴ� �̺�Ʈ�� ���� ���� ����


	��, �Ű����� s �� ���޵� �ڵ��� ���Ͽ���
	INetworkEvents �� ���޵� �̺�Ʈ �� �ϳ��� �߻��ϸ�
	hEventObjecdt �� ���޵� Ŀ�� ������Ʈ�� signaled ���·� �ٲٴ� �Լ�

3��° ���� ����
1) FD_READ   : ������ �����Ͱ� �����ϴ°�
1) FD_WRITE  : ���ŷ ���� ������ ������ �����Ѵ�.
1) FD_OOB    : Out-of-band �����Ͱ� ���ŵǾ��°�
1) FD_ACCEPT : ���� ��û�� �־��°�
1) FD_CLOSE  : ���� ���ᰡ ��û�Ǿ��°�
----------------------------------------------------------------------
WSACreateEvent
- manual reset + non-signaled �̺�Ʈ ������Ʈ ���� �Լ�

WSACloseEvent
- ������ ������� �̺�Ʈ ������Ʈ �Ҹ��Ű�� �Լ�

DWORD WSAWaitForMultipleEvents(WORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
- cEvents : signaled ���·��� ���̿��θ� Ȯ���� Event ������Ʈ ����  -> �ִ� 64��
- lphEvents : signaled ���·��� ���̿��θ� Ȯ���� Event ������Ʈ �迭 �ּҰ�
- fWaitAll : TRUE �� ��� Event ������Ʈ�� signaled ���°� �Ǿ�� ��ȯ, FALSE �� �ϳ��� �Ǿ ��ȯ
- dwTimeout : 1/1000 �� ���� ����, WSA_INFINITE ���޽� signaled ���°� �� ������ ��ȯ���� �ʴ´�.
              0�� �����ϸ� �Լ�ȣ��� ���ÿ� ��ȯ�� �̷�����. (��, Event ������Ʈ�� manual-reset ��忩�߸� �����ϴ�)
- fAlertable : TRUe ���� ��, alertable ���·��� ���� 
- ��ȯ�� : WSA_WAIT_EVENT_0�� ����, signaled ���°� �� Event ������Ʈ�� �ڵ��� ����� �ε��� ��� , ���� �� �̻� ? �� ���� �ε��� �� ���
           ���� Ÿ�Ӿƿ� �߻��ϸ� WAIT_TIMEOUT ��ȯ

- �̺�Ʈ Ŀ�� ������Ʈ�� ���� �̺�Ʈ �߻� ������ Ȯ���ϴ� �Լ�
*/