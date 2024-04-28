#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 해당 주소가, 메일 슬롯의 주소이다.
#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	// mailSlot 핸들
	HANDLE hMailSlot;

	TCHAR messageBox[50];

	DWORD bytesRead;

	// mail slot 생성
	hMailSlot = CreateMailslot(
		SLOT_NAME,
		0,
		MAILSLOT_WAIT_FOREVER,
		NULL
	);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot\n"), stdout);
		return 1;
	}

	// message 수신
	_fputts(_T("**** Message ****"), stdout);

	while (1)
	{
		if (!ReadFile(
			hMailSlot,  // 메일 슬롯 핸들
			messageBox,  // 읽은 데이터 저장할 버퍼
			sizeof(TCHAR) * 50, // 읽어 들일 버퍼 최대 크기
			&bytesRead, // 읽어들인 데이터 크기를 바이트 단위로 얻기
			NULL))
		{
			_fputts(_T("Unable to read!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcsncmp(messageBox, _T("exit"), 4))
		{
			_fputts(_T("Good Bye !"), stdout);
			break;
		}

		// null 문자 삽입
		messageBox[bytesRead / sizeof(TCHAR)] = 0;
		_fputts(messageBox, stdout);
	}

	CloseHandle(hMailSlot);

	return 0;
}