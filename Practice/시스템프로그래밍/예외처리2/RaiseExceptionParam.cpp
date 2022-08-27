#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// ����� (������) ���� ����
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
			// ���� ���� 3��
			3,
			// ���� ���
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
    DWORD    ExceptionCode; // RaiseException.dwExceptionCode �� �ش�
    DWORD ExceptionFlags; // RaiseException.dwExceptionFlags �� �ش�
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters; // RaiseException.nNumberOfArguments �� �ش�
    ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS]; // RaiseException.lpArguments �� �ش�
    } EXCEPTION_RECORD;
	
	
	RaiseException(
    _In_ DWORD dwExceptionCode,
    _In_ DWORD dwExceptionFlags,

	// int main() �� argc �� ����
    _In_ DWORD nNumberOfArguments,
	// int main() �� char* argv[] ����
    _In_reads_opt_(nNumberOfArguments) CONST ULONG_PTR* lpArguments
    );

	���� Ư�� ���� ��Ȳ �߻� �ÿ�
	���ܿ� ���õ� ������ ��Ƽ�
	����ó�� ������ �����ϰ� �ʹٸ�
	RaiseException �Լ��� 
	3��°, 4��° �������ڸ� Ȱ���ϸ� �ȴ�.

	�̷��� �ϸ� �ش� ���ڵ���
	EXCEPTION_RECORD ����ü��
	5��°, 6��° ��� ���� ä��� �ȴ�.

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
