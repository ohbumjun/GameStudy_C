#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 핸들의 상속 확인용 예제
int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR message[50];
	DWORD bytesWritten;

	// 핸들을 얻는 코드
	// 파일을 열어서, 부모 프로세스가 실행 중에 저장해 놓은
	// 핸들 정보를 참조하고 있다.
	// 이제 이 핸들을 이용해서 부모 프로세스와 동일한 형태로
	// 메일 슬롯에 메시지를 전달할 수 있다.
	FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("rt"));
	_ftscanf(file, _T("%d"), &hMailSlot);
	fclose(file);
	_tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

	while (1)
	{
		_fputts(_T("My CMD >"), stdout);
		_fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

		if (!WriteFile(
			hMailSlot,
			message, // 전송할 데이터가 저장되어 있는 버퍼
			_tcslen(message) * sizeof(TCHAR),
			&bytesWritten, // 함수 호출 후 전송된 데이터 크기
								  // 바이트 단위로 얻어오기 
			NULL))
		{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye !"), stdout);
			break;
		}

		CloseHandle(hMailSlot);
		return 0;
	}
}