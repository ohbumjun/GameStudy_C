// ����Ʈ���� ����
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

	// 30, 31 ��Ʈ : ������ �ɰ��� ����
	// - ���� ���� ���� ��Ȳ�̶�� ����
	DefinedException |= 0x01 << 31;
	DefinedException |= 0x01 << 30;

	// MS or Customer
	// - 29 ��Ʈ : ���ܸ� ������ ��ü�� ���� ����
	// 0 : ����ũ�� ����Ʈ�� ������ ����
	// 1 : �Ϲ� �����ڰ� ������ ����
	DefinedException |= 0x00 << 29;

	// - 28 ��Ʈ : �ý��ۿ� ���� ����Ǿ� �ִ� ��Ʈ
	// 0���� �ʱ�ȭ�� ����
	DefinedException |= 0x00 << 28;

	// - 16 ~ 27 ��Ʈ : ���ܹ߻� ȯ�� ����
	// ex) FACILITY_INTERNET : value
	// ex) ���ͳݰ� ���õ� ����
	DefinedException |= 0x00 << 16;

	// 0 ~ 15 : ���� ���� ���� �뵵
	// - �����ϰ� �츮�� ������ ������ ���⿡ �ش�
	// - ���Ⱑ ���ܸ� �����ϴ� �ٽ��� �ȴ�.
	DefinedException |= 0x08;

	__try
	{
		_tprintf(_T("Send Exception Code : 0x%x\n"), DefinedException);
	
		RaiseException(
			// �߻���ų ���� ���� ����
			DefinedException, 
			// ���� �߻� ���� �����Ŀ� �־ ������ �� �� ���
			0, 
			// �߰������� ����
			NULL, 
			// �߰� ���� ����
			NULL
		);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
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
		
		_tprintf(_T("Receive Exception Code : 0x%x\n"), exptType);

	}
}

*/