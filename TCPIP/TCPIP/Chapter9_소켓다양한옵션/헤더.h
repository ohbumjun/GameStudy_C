#pragma once

/*
소켓의 특성을 변경할 필요가 있다.

-----------------------------------------------------------------

>> 소켓의 옵션을 확인하는 함수

int getsockopt(SOCKET sock, 
		int level, int optname, char* optval, int* optlen);
		- sock  : 옵션확인을 위한 소켓 핸들 전달
		- level : 확인할 옵션의 프로토콜 레벨 전달
		- optname : 확인할 옵션의 이름 전달
		- optval : 확인결과 저장을 위한 버퍼 주소값
		- optlen : 4번째 매개변수 optval 로 전달된 주소 값의 
		  버퍼 크기를 담고 있는 변수의 주소값 전달
		  호출 완료되면, 해당 변수에는 4번째 인자를 통해 반환된 
		  옵션정보의 크기가 바이트 단위로 계산되어 저장된다.

>> 소켓의 옵션을 변경하는 함수

int setsockopt(SOCKET sock, 
		int level, int optname, const char* optval, int optlen);
		- sock    : 옵션변경을 위한 소켓 핸들 전달
		- level   : 변경할 옵션의 프로토콜 레벨 전달
		- optname : 변경할 옵션의 이름 전달
		- optval  : 변경할 옵션정보를 저장한 버퍼의 주소값 전달
		- optlen  : 4번째 매개변수 optval 로 전달된 옵션정보의
		            바이트 단위 크기 전달
		
*/