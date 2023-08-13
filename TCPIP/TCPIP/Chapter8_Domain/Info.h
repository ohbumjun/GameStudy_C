#pragma once

/*
>> ���ڿ� ������ ������ �̸����κ��� IP  �ּ� ���� ������
struct hostent* gethostbyname(const char* hostname);

struct hostent
{
	char* h_name; // ���� ������ �̸�
	char** h_aliases; // �ϳ��� IP�� �� �̻��� ������ �̸� ���� ����
	int h_addrtype; // h_addr_list �� ��ȯ�� IP�ּ��� �ּ�ü�迡 ���� ���� ex) IPv4 : AF_INET, IPv6 
	int h_length;   // ȣ������ ��ȯ�� IP �ּ� ũ�� ���� -> IPv4�� 4����Ʈ, IPv6 �� 16
	char** h_addr_list; // IP�ּҰ� ���� ���·� ��ȯ -> �ϳ��� ������ �̸��� �����Ǵ� �������� IP �ּ� ������ ���� �� �ִ�.
}

>> IP�ּ� -> ������ �̸�
struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family)
- len    : ù��° ���ڷ� ���޵� �ּ� ���� ���� -> IPv4 : 4, IPv6 : 16.
- family : �ּ�ü�� ���� ����. IPv4�� AF_INET , IPv6�� AF_INET6
*/