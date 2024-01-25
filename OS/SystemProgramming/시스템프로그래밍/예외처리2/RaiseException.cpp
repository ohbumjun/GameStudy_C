// 소프트웨어 예외
/*

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

void SoftwareException(void);

int _tmain(int argc, TCHAR* argv[])
{
	SoftwareException();

	_tprintf(_T("---THE END---"));

	return 0;
};

void SoftwareException()
{

	DWORD DefinedException = 0x00;

	// 30, 31 비트 : 예외의 심각도 수준
	// - 강도 높은 오류 상황이라고 정의
	DefinedException |= 0x01 << 31;
	DefinedException |= 0x01 << 30;

	// MS or Customer
	// - 29 비트 : 예외를 정의한 주체에 대한 정보
	// 0 : 마이크로 소프트가 정의한 예외
	// 1 : 일반 개발자가 정의한 에외
	DefinedException |= 0x00 << 29;

	// - 28 비트 : 시스템에 의해 예약되어 있는 비트
	// 0으로 초기화한 상태
	DefinedException |= 0x00 << 28;

	// - 16 ~ 27 비트 : 예외발생 환경 정보
	// ex) FACILITY_INTERNET : value
	// ex) 인터넷과 관련된 예외
	DefinedException |= 0x00 << 16;

	// 0 ~ 15 : 예외 종류 구분 용도
	// - 순수하게 우리가 정의할 내용이 여기에 해당
	// - 여기가 예외를 구분하는 핵심이 된다.
	DefinedException |= 0x08;

	__try
	{
		_tprintf(_T("Send Exception Code : 0x%x\n"), DefinedException);
	
		RaiseException(
			// 발생시킬 예외 형태 지정
			DefinedException, 
			// 예외 발생 이후 실행방식에 있어서 제한을 둘 때 사용
			0, 
			// 추가정보의 개수
			NULL, 
			// 추가 정보 전달
			NULL
		);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DWORD exptType = GetExceptionCode();
		
		// 개발자가 발생시킨 예외
		if (exptType & (0x01 << 29))
		{

		}
		else
		{
		// 마이크로 소프트 운영체제 예외

		}
		
		_tprintf(_T("Receive Exception Code : 0x%x\n"), exptType);

	}
}

*/