#pragma once

/*
>> 문자열 형태의 도메인 이름으로부터 IP  주소 정보 얻어오기
struct hostent* gethostbyname(const char* hostname);

struct hostent
{
	char* h_name; // 공식 도메인 이름
	char** h_aliases; // 하나의 IP에 둘 이상의 도메인 이름 지정 가능
	int h_addrtype; // h_addr_list 로 반환된 IP주소의 주소체계에 대한 정보 ex) IPv4 : AF_INET, IPv6 
	int h_length;   // 호출결과로 반환된 IP 주소 크기 정보 -> IPv4는 4바이트, IPv6 는 16
	char** h_addr_list; // IP주소가 정수 형태로 변환 -> 하나의 도메인 이름에 대응되는 여러개의 IP 주소 정보를 얻을 수 있다.
}

>> IP주소 -> 도메인 이름
struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family)
- len    : 첫번째 인자로 전달된 주소 정보 길이 -> IPv4 : 4, IPv6 : 16.
- family : 주소체계 정보 전달. IPv4는 AF_INET , IPv6는 AF_INET6
*/