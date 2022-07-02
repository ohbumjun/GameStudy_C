#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR command[] = _T("Operation2.exe");

	CreateProcess(NULL, command, NULL, NULL, TRUE,
		0, NULL, NULL, &si, &pi);

	DWORD timing = 0;

	while (1)
	{
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD j = 0; j < 10000; ++j){}

		_fputts(_T("Parent \n"), stdout);

		timing += 1;

		// Operation2.exe 에서는 자신 프로세스의 우선순위를 높여준다
		// 그리고 KerObjProcess.exe 에서 Operation2.exe 의 프로세스
		// 커널 오브젝트에 접근해서, 다시 우선순위를 되돌려 둔다.
		if (timing == 2)
			SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
	}
}