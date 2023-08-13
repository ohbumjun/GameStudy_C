#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv)
{
	HANDLE hProcess;
	TCHAR cmdString[1024];

	// GetCurrentProcess 함수를 통해서
	// 얻어내는 가짜 핸들을
	// 진짜 핸들로 구성해서
	// 핸들 테이블에 등록하고 있다.
	DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentProcess(),
		GetCurrentProcess(),
		&hProcess, 0,
		TRUE, // 상속 가능한 핸들로 만들어라
		DUPLICATE_SAME_ACCESS
	);

	// 얻어낸 핸들 정보를
	// 문자열로 구성한다.
	_stprintf(
		cmdString,
		_T("%s %u"),
		_T("ChildProcess.exe"),
		(unsigned)hProcess
	);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	// 새로운 콘솔창을 열어
	// 자식 프로세스 생성
	// 위에서 문자열로 구성한 핸들값을
	// 자식 프로세스의 main 함수 인자로 전달한다.
	BOOL isSuccess =
		CreateProcess(
			NULL, 
			cmdString, 
			NULL, 
			NULL, 
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL, 
			NULL,
			&si, &pi
		);

	if (isSuccess == FALSE)
	{
		_tprintf(_T("CreateProcess failed\n"));
		return -1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_tprintf(_T("Parent Process\n"));
	_tprintf(_T("ooooops!"));

	return 0;
}