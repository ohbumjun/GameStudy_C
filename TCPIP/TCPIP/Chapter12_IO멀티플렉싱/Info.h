#pragma once

/*
FD_ZERO(fd_set * fdset)
- fd_set 변수의 모든 비트 0으로

FD_SET(int fd, fd_set * fdset)
- 해당 비트에 해당하는 파일 디스크립터 관찰대상으로

FD_CLR(int fd, fd_set * fdset)
- 해당 비트에 해당하는 파일 디스크립터 관찰대상 해제

FD_ISSET(int fd, fd_set * fdset)
- 해당 비트에 해당하는 파일 디스크립터 관찰 대상 여부 확인

-----------------------------------------------------------

int select(
	int maxfd, fd_set* readset, fd_set* writeset,
	fd_set* exceptset, const struct timeval* timeout);

	- maxfd     : 검사 대상이 되는 파일 디스크립터 수
	- readset   : fd_set 형 변수에 "수신된 데이터의 존재 여부" 에 
	             관심있는 파일 디스크립터 정보를 등록해서 해당 변수의 주소값 전달
	- writeset  : fd_set 형 변수에 "블로킹 없는 데이터 전송의 가능여부" 에 
	             관심있는 파일 디스크립터 정보를 등록해서 해당 변수의 주소값 전달
	- exceptset : fd_set 형 변수에 "예외상황 발생 여부" 에
				 관심있는 파일 디스크립터 정보를 등록해서 해당 변수의 주소값 전달
	- timeout   : select 함수 호출 이후 무한정 블로킹 상태에 빠지지 않도록 타임 아웃(time-out)을 설정하기 위한 인자 전달

select 함수는 수신, 전송, 예외 라는 3가지 관찰항목의 변화를 확인한다.
각 항목 별로 fd_set형 변수를 선언하여 파일 디스크립터를 등록하고, 이 변수의 주소값을
위 함수의 2,3,4 번째 변수로 전달한다.

*/