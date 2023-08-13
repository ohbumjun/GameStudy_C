#pragma once

/*
size_t send (int sockfd, const void* buf, size_t nbytes, int flags);

size_t recv (int sockfd, const void* buf, size_t nbytes, int flags);
- flags : 데이터 전송시 적용할 다양한 옵션 정보

MSG_OOB       : 긴급 데이터(Out-of-band data)의 전송을 위한 옵션
- 응급환자를 위한 응급실이 따로 존재하듯이
   긴급으로 무엇인가를 처리하려면 처리방법 및 경로가 달라야 한다.
MSG_PEEK      : 입력 버퍼에 수신된 데이터의 존재유무를 확인하기 위한 옵션
MSG_DONTROUTE : 데이터 전송 과정에서 라우팅 테이블 참조 X, 따라서 로컬 네트워크상에서 목적지 찾으 때 사용
MSG_DONTWAIT  : 입출력 함수 호출과정에서 블로킹 되지 않을 것을 요구 즉, 넌블로킹 IO의 요구에 사용되는 옵션
MSG_WAITALL   : 요청한 바이트 숭 해당하는 데이터가 전부 수신될 때까지 호출된 함수가 반환되는 것을 막기 위한 옵션

----------------------------------------------------------------------------------------------------------------
readv & writev

struct iovec
{
	void* ios_base // 버퍼 주소 정보
	size_t iov_len // 버퍼 크기 정보
}

size_t writev (int filedes, const struct iovec* iov, int iovcnt);
- filedes : 데이터 전송의 목적지를 나타내는 소켓의 파일 디스크립터
- iov     : 전송할 데이터를 모아둔, 구조체 iovec 배열의 주소값 전달. 
- iovcnt  : 두번째 인자로 전달된 주소값이 가리키는 배열의 길이 정보 전달 

size_t readv (int filedes, const struct iovec* iov, int iovcnt);
- filedes : 데이터 수신할 소켓의 파일 디스크립터
- iov     : 데이터를 저장할 위치와 크기 정보를 담고 있는 구조체 iovec 배열의 주소값 전달.
- iovcnt  : 두번째 인자로 전달된 주소값이 가리키는 배열의 길이 정보 전달
*/