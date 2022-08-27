// 소프트웨어 예외
/*

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BEST_TEST_EXCEPTION ((DWORD)0xE0000008L)

void SoftwareException(void);

int _tmain(int argc, TCHAR* argv[])
{
	SoftwareException();

	_tprintf(_T("---THE END---"));

	return 0;
};

void SoftwareException()
{
	__try
	{

		RaiseException(
			// 발생시킬 예외 형태 지정
			BEST_TEST_EXCEPTION,
			// 예외 발생 이후 실행방식에 있어서 제한을 둘 때 사용
			EXCEPTION_NONCONTINUABLE, // 예외 발생 이후 더이상의 실행을 막는다 -> EXCEPTION_CONTINUE_EXECUTION 의 흐름을 막기 위해 사용
			// 추가정보의 개수
			NULL,
			// 추가 정보 전달
			NULL
		);
		
		_tprintf(_T("It Works\n"));
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
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

		if (exptType == BEST_TEST_EXCEPTION)
		{
			_tprintf(_T("BEST_TEST_EXCEPTION Error Occured \n"), exptType);

		}
	}
}

*/