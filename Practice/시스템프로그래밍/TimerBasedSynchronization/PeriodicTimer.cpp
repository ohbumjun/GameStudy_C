// 주기적 타이머

// 해당 코드를 넣어야 한다. 왜냐하면
// CreateWaitableTimer, SetWaitableTimer 함수가
// 선언되도록 헤더 파일 내부에 조건부 컴파일 형태를 취하고 있기 때문
// 당연한 얘기이지만, 헤더 파일 선언 이전에 와야 한다.
#define _WIN32_WINNT 0x400

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000000;

	// 타이머 오브젝트 생성
	hTimer = CreateWaitableTimer(
		NULL, // 보안 속성 -> 핸들을 자식 프로세스에게 상속하고자 하는 경우, NULL 이 아닌 다른 값 전달
		FALSE, // 타이머 오브젝트에 대한 수동 리셋 모드, 자동 리셋 모드 
					// 주의할 점은, 수동 리셋 타이머, 주기적 타이머를 설정하는 코드가 아니라는 것이다.
		_T("Waitable Timer") // 타이머 오브젝트 이름
	);

	if (!hTimer)
	{
		_tprintf(_T("CreateWaitable Timer Failed (%d) \n"),
			GetLastError());

		return 1;
	}

	_tprintf(_T("Waiting For 10 sec...\n"));

	// 타이머의 알람을 10초로 설정
	SetWaitableTimer(
		hTimer, // 알람을 설정할 타이머 오브젝트의 핸들

		// 알람울리는 시간 조절 
		// + 값을 전달하면, 절대 시간을 의미하게 되고
		// - 값이 전달되면 상대시간을 의미하게 된다.
		// 즉, A시 B분에 알람을 울려주세요 같이, 직접 시간을 전달할 경우, +
		// 지금으로부터 A초 후에 울려주세요. 같이 요청을 할 경우 - 을 전달
		// 1000만 분의 1초 단위로 시간을 설정한다.
		&liDueTime,

		// 타이머가 주기적으로 알람을 울리게할 때 사용하는 전달인자
		// 0을 전달할 경우, 주기적인 알람을 사용하지 않겠다는 의미가 된다.
		// 주기적 타이머를 설정할 경우, 타이머 오브젝트는 반드시 "자동 리셋 모드"로 설정해야 한다.
		// 그래야 WaitForSingleObject 함수를 지나갈 때마다, 해당 타이머 오브젝트가 다시
		// Signaled 상태가 되고, 
		// 이로 인해 Timer 가 주기적으로 반응할 수 있기 때문이다.
		5000,
		NULL,
		NULL,
		FALSE
	);

	// 타이머의 알람을 대기
	while (1)
	{
		WaitForSingleObject(hTimer, INFINITE);
		_tprintf(_T("Timer was signaled.\n"));
		MessageBeep(MB_ICONEXCLAMATION);
	}

	return 0;
}