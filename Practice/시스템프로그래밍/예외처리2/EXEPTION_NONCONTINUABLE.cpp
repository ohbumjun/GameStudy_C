// ����Ʈ���� ����
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
			// �߻���ų ���� ���� ����
			BEST_TEST_EXCEPTION,
			// ���� �߻� ���� �����Ŀ� �־ ������ �� �� ���
			EXCEPTION_NONCONTINUABLE, // ���� �߻� ���� ���̻��� ������ ���´� -> EXCEPTION_CONTINUE_EXECUTION �� �帧�� ���� ���� ���
			// �߰������� ����
			NULL,
			// �߰� ���� ����
			NULL
		);
		
		_tprintf(_T("It Works\n"));
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
		DWORD exptType = GetExceptionCode();

		// �����ڰ� �߻���Ų ����
		if (exptType & (0x01 << 29))
		{

		}
		else
		{
			// ����ũ�� ����Ʈ �ü�� ����

		}

		if (exptType == BEST_TEST_EXCEPTION)
		{
			_tprintf(_T("BEST_TEST_EXCEPTION Error Occured \n"), exptType);

		}
	}
}

*/