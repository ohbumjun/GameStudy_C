#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s' 는 실행할 수 있는 프로그램이 아닙니다\n");

int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

using namespace std;

int main()
{
	// 한글 입력을 가능하게 하기 위해
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit; // unsigned long -> Window 32, 64 에서는 둘다 4byte

	while (1)
	{
		isExit = CmdProcessing();

		if (isExit == TRUE) // TRUE : 1 , FALSE : 0 => 즉 단순 숫자
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}

	return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(", \t\n");


// CmdProcessing();
// 기능 : 명령어를 입력 받아서 해당 명령어에 지정되어 있는 기능을 수행한다.
// exit 가 입력되면, TRUE 를 반환하고 이는 프로그램의 종료로 이어진다.
int CmdProcessing()
{
	_fputts(_T("Best Command prompt >> "), stdout);

	// _getws_s : stdin 스트림에서 줄을 가져온다.
	_getts_s(cmdString);

	// 1번째 문자열 , 2번째 : 구분 기호 문자
	// 즉, 1번째 문자열에 서 발견된 토큰에 대한 포인터 반환
	// 이 경우, Enter, 혹은 Tab 이전까지의 문자 내용을 가져오는 것 같다.
	TCHAR* token = _tcstok(cmdString, seps);

	int tokenNum = 0;

	// _tctok 을 통해, 더이상 토큰을 찾을 수 없다면 NULL 이 반환된다.
	// NULL이 아니라는 것은, 발견한 토큰이 존재한다는 것이다.
	while (token != NULL)
	{
		// StrLower : 모든 대문자를 소문자로 변경한다.
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		// 그 다음 Token을 찾는다.
		token = _tcstok(NULL, seps);
	}

	// exit을 입력했다면 Break
	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("추가 명령어 1")))
	{
		
	}
	else if (!_tcscmp(cmdTokenList[0], _T("추가 명령어 2")))
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