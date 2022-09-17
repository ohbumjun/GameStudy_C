#pragma once

/*
size_t send (int sockfd, const void* buf, size_t nbytes, int flags);

size_t recv (int sockfd, const void* buf, size_t nbytes, int flags);
- flags : ������ ���۽� ������ �پ��� �ɼ� ����

MSG_OOB       : ��� ������(Out-of-band data)�� ������ ���� �ɼ�
- ����ȯ�ڸ� ���� ���޽��� ���� �����ϵ���
   ������� �����ΰ��� ó���Ϸ��� ó����� �� ��ΰ� �޶�� �Ѵ�.
MSG_PEEK      : �Է� ���ۿ� ���ŵ� �������� ���������� Ȯ���ϱ� ���� �ɼ�
MSG_DONTROUTE : ������ ���� �������� ����� ���̺� ���� X, ���� ���� ��Ʈ��ũ�󿡼� ������ ã�� �� ���
MSG_DONTWAIT  : ����� �Լ� ȣ��������� ���ŷ ���� ���� ���� �䱸 ��, �ͺ��ŷ IO�� �䱸�� ���Ǵ� �ɼ�
MSG_WAITALL   : ��û�� ����Ʈ �� �ش��ϴ� �����Ͱ� ���� ���ŵ� ������ ȣ��� �Լ��� ��ȯ�Ǵ� ���� ���� ���� �ɼ�

----------------------------------------------------------------------------------------------------------------
readv & writev

struct iovec
{
	void* ios_base // ���� �ּ� ����
	size_t iov_len // ���� ũ�� ����
}

size_t writev (int filedes, const struct iovec* iov, int iovcnt);
- filedes : ������ ������ �������� ��Ÿ���� ������ ���� ��ũ����
- iov     : ������ �����͸� ��Ƶ�, ����ü iovec �迭�� �ּҰ� ����. 
- iovcnt  : �ι�° ���ڷ� ���޵� �ּҰ��� ����Ű�� �迭�� ���� ���� ���� 

size_t readv (int filedes, const struct iovec* iov, int iovcnt);
- filedes : ������ ������ ������ ���� ��ũ����
- iov     : �����͸� ������ ��ġ�� ũ�� ������ ��� �ִ� ����ü iovec �迭�� �ּҰ� ����.
- iovcnt  : �ι�° ���ڷ� ���޵� �ּҰ��� ����Ű�� �迭�� ���� ���� ����
*/