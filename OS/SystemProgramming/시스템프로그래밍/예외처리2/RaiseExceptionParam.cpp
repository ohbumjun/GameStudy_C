#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 사용자 (개발자) 정의 예외
#define STATUS_DEFAULT_USER_DEFINED_EXPT ((DWORD)0xE0000008L)

void SoftwareException(void);
DWORD FilterFunction(LPEXCEPTION_POINTERS exptPrt);

int _tmain(int argc, TCHAR* argv[])
{
	SoftwareException();

	_tprintf(_T("End of the _tmain \n"));

	return 0;
};

void SoftwareException(void)
{
	const TCHAR* exptString[] = {
		_T("Exception String One !"),
		_T("Exception String Two !"),
		_T("Exception String Three !")
	};

	__try
	{
		RaiseException(
			STATUS_DEFAULT_USER_DEFINED_EXPT,
			0,
			// 인자 개수 3개
			3,
			// 인자 목록
			(ULONG_PTR*)exptString
		);
	}
	__except (FilterFunction(GetExceptionInformation()))
	{
		DWORD exptType = GetExceptionCode();
	
		_tprintf(_T("Receive Exception Code : 0x%x\n"), exptType);

	}
}

DWORD FilterFunction(LPEXCEPTION_POINTERS exptPtr)
{
	PEXCEPTION_RECORD exptRecord = exptPtr->ExceptionRecord;

	/*
	typedef struct _EXCEPTION_RECORD {
    DWORD    ExceptionCode; // RaiseException.dwExceptionCode 에 해당
    DWORD ExceptionFlags; // RaiseException.dwExceptionFlags 에 해당
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters; // RaiseException.nNumberOfArguments 에 해당
    ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS]; // RaiseException.lpArguments 에 해당
    } EXCEPTION_RECORD;
	
	
	RaiseException(
    _In_ DWORD dwExceptionCode,
    _In_ DWORD dwExceptionFlags,

	// int main() 의 argc 의 역할
    _In_ DWORD nNumberOfArguments,
	// int main() 의 char* argv[] 역할
    _In_reads_opt_(nNumberOfArguments) CONST ULONG_PTR* lpArguments
    );

	만약 특정 예외 상황 발생 시에
	예외와 관련된 정보를 담아서
	예외처리 영역에 전달하고 싶다면
	RaiseException 함수의 
	3번째, 4번째 전달인자를 활용하면 된다.

	이렇게 하면 해당 인자들은
	EXCEPTION_RECORD 구조체의
	5번째, 6번째 멤버 들을 채우게 된다.

	*/

	switch (exptRecord->ExceptionCode)
	{
	case STATUS_DEFAULT_USER_DEFINED_EXPT :
	{
		_tprintf(_T("STATUS_DEFAULT_USER_DEFINED_EXPT exception\n"));

		_tprintf(_T("Exception code : 0x%x \n"),
			exptRecord->ExceptionCode);

		_tprintf(_T("Exception flags : %d \n"),
			exptRecord->ExceptionFlags);

		_tprintf(_T("Exception param num : %d \n"),
			exptRecord->NumberParameters);

		_tprintf(_T("String one : %s \n"),
			(TCHAR*)exptRecord->ExceptionInformation[0]);

		_tprintf(_T("String two : %s \n"),
			(TCHAR*)exptRecord->ExceptionInformation[1]);

		_tprintf(_T("String three : %s \n"),
			(TCHAR*)exptRecord->ExceptionInformation[2]);

		break;
	}
	default :
		break;
	
	}


	return EXCEPTION_EXECUTE_HANDLER;
}
