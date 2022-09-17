#pragma once

/*

int WSAEventSelect(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);

	- 성공시 0, 실패시 SOCKET_ERROR 반환
	- s : 관찰 대상 소켓의 핸들
	- hEventObject  : 이벤트 발생 유무의 확인을 위한 Event 오브젝트의 핸들 전달
	- INetworkEvent : 감시하고자 하는 이벤트의 유형 정보 전달


	즉, 매개변수 s 에 전달된 핸들의 소켓에서
	INetworkEvents 에 전달된 이벤트 중 하나가 발생하면
	hEventObjecdt 에 전달된 커널 오브젝트를 signaled 상태로 바꾸는 함수

3번째 인자 종류
1) FD_READ   : 수신할 데이터가 존재하는가
1) FD_WRITE  : 블로킹 없이 데이터 전송이 가능한다.
1) FD_OOB    : Out-of-band 데이터가 수신되었는가
1) FD_ACCEPT : 연결 요청이 있었는가
1) FD_CLOSE  : 연결 종료가 요청되었는가
----------------------------------------------------------------------
WSACreateEvent
- manual reset + non-signaled 이벤트 오브젝트 생성 함수

WSACloseEvent
- 위에서 만들어진 이벤트 오브젝트 소멸시키는 함수

DWORD WSAWaitForMultipleEvents(WORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
- cEvents : signaled 상태로의 전이여부를 확인할 Event 오브젝트 개수  -> 최대 64개
- lphEvents : signaled 상태로의 전이여부를 확인할 Event 오브젝트 배열 주소값
- fWaitAll : TRUE 시 모든 Event 오브젝트가 signaled 상태가 되어야 반환, FALSE 면 하나만 되어도 반환
- dwTimeout : 1/1000 초 단위 지정, WSA_INFINITE 전달시 signaled 상태가 될 때까지 반환하지 않는다.
              0을 전달하면 함수호출과 동시에 반환이 이뤄진다. (단, Event 오브젝트가 manual-reset 모드여야만 가능하다)
- fAlertable : TRUe 전달 시, alertable 상태로의 진입 
- 반환값 : WSA_WAIT_EVENT_0을 빼면, signaled 상태가 된 Event 오브젝트의 핸들이 저장된 인덱스 계산 , 만약 둘 이상 ? 더 작은 인덱스 값 계산
           만일 타임아웃 발생하면 WAIT_TIMEOUT 반환

- 이벤트 커널 오브젝트를 통해 이벤트 발생 유무를 확인하는 함수
*/