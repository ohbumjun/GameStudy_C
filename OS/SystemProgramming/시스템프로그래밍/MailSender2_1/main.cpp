#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 해당 주소가, 메일 슬롯의 주소이다.
#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR message[50];

	// number of bytes write
	DWORD bytesWritten;

	// 프로세스 상속 세팅
	// - 핸들이 상속 가능하도록 세팅
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// 메시지 보내기
	hMailSlot = CreateFile(
		SLOT_NAME,
		GENERIC_WRITE,  // 사용되는 용도 
									// fopen 의 w, r, wb, rb 같은 거
		FILE_SHARE_READ,
		&sa, // 상속 가능
		OPEN_EXISTING, // 생성 방식
								// 새로운 파일 생성 or 기존 파일 접근
								// 이미 만들어놓은 메일 슬롯에 접근
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailSlot!\n"), stdout);
		return 1;
	}

	_tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

	// Handle 정보를 File 형태로 저장
	FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("wt"));
	_ftprintf(file, _T("%d"), hMailSlot);
	fclose(file);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR command[] = _T("MailSender2_2.exe");

	// create_new_console
	// 자식 프로세스에게 새로운 콘솔 할당
	CreateProcess(
	NULL, command,
		NULL, NULL,
		TRUE, // 핸들 테이블 상속 설정
		CREATE_NEW_CONSOLE,
		NULL, NULL,
		&si, &pi
	);
 
	while (1)
	{
		_fputts(_T("MY CMD >"), stdout);
		_fgetts(message,
			sizeof(message) / sizeof(TCHAR),
			stdin);

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
	}

	CloseHandle(hMailSlot);

	return 0;
}