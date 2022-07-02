#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s' �� ������ �� �ִ� ���α׷��� �ƴմϴ�\n");

int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

using namespace std;

int main()
{
	// �ѱ� �Է��� �����ϰ� �ϱ� ����
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit; // unsigned long -> Window 32, 64 ������ �Ѵ� 4byte

	while (1)
	{
		isExit = CmdProcessing();

		if (isExit == TRUE) // TRUE : 1 , FALSE : 0 => �� �ܼ� ����
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}

	return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(", \t\n");


// CmdProcessing();
// ��� : ��ɾ �Է� �޾Ƽ� �ش� ��ɾ �����Ǿ� �ִ� ����� �����Ѵ�.
// exit �� �ԷµǸ�, TRUE �� ��ȯ�ϰ� �̴� ���α׷��� ����� �̾�����.
int CmdProcessing()
{
	_fputts(_T("Best Command prompt >> "), stdout);

	// _getws_s : stdin ��Ʈ������ ���� �����´�.
	_getts_s(cmdString);

	// 1��° ���ڿ� , 2��° : ���� ��ȣ ����
	// ��, 1��° ���ڿ��� �� �߰ߵ� ��ū�� ���� ������ ��ȯ
	// �� ���, Enter, Ȥ�� Tab ���������� ���� ������ �������� �� ����.
	TCHAR* token = _tcstok(cmdString, seps);

	int tokenNum = 0;

	// _tctok �� ����, ���̻� ��ū�� ã�� �� ���ٸ� NULL �� ��ȯ�ȴ�.
	// NULL�� �ƴ϶�� ����, �߰��� ��ū�� �����Ѵٴ� ���̴�.
	while (token != NULL)
	{
		// StrLower : ��� �빮�ڸ� �ҹ��ڷ� �����Ѵ�.
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		// �� ���� Token�� ã�´�.
		token = _tcstok(NULL, seps);
	}

	// exit�� �Է��ߴٸ� Break
	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("�߰� ��ɾ� 1")))
	{
		
	}
	else if (!_tcscmp(cmdTokenList[0], _T("�߰� ��ɾ� 2")))
	{

	}
	else
	{
		_tprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}

TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
		{
			*pStr = _totlower(*pStr);
		}
		++pStr;
	}

	return ret;
}