#include <stdio.h>
#include <tchar.h>
#include <windows.h>

TCHAR strData[] =
_T("A\n")
_T("B\n")
_T("C\n")
_T("D\n")
_T("E\n");

// 완료루틴 I/O 함수 호출 양식
VOID WINAPI FILEIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);


int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("data.txt");

	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_WRITE,
		0, CREATE_ALWAYS, 
		FILE_FLAG_OVERLAPPED, // 중첩 I/O 의 확장 
		0
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File creation fault !\n"));
		return -1;
	}

	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));

	// 완료 루틴 I/O 의 경우, 중첩 I/O 와 달리
	// 이벤트 오브젝트가 필요하지 않다.
	overlappedInst.hEvent = (HANDLE)1234;

	WriteFileEx(hFile, // 데이터 수신 대상
		strData, // 데이터 수신을 위한 버퍼 주소
		sizeof(strData),  // 읽어들일 데이터의 최대 크기
		&overlappedInst,  
		FILEIOCompletionRoutine); // 입력 연산 완료시 자동으로 호출되는 완료루틴

	// 첫번째 인자 : 쓰레드의 실행을 멈추는데 사용하는 것
	// 두번째 인자 : FALSE 가 되면, Sleep 함수와 동일
	// TRUE 라면, 해당 함수를 호출한 쓰레드를 알림 가능한 상태로 변경
	// 즉, 호출되어야할 완료 루틴의 호출이 이어지는 것이고
	// 완료 루틴 실행 후에는 SleepEx 함수도 반환하게 된다.
	SleepEx(INFINITE, TRUE);
		
	CloseHandle(hFile);
	return 1;
};


VOID WINAPI FILEIOCompletionRoutine(DWORD errorCode, 
	DWORD numOfBytesTransfered, 
	LPOVERLAPPED overlapped)
{
	// errorCode : 별도로 GetLastError 함수를 호출할 필요가 없다.
	// 해당 값이 0 이라는 것은, 에러가 발생하지 않았음을 의미
	_tprintf(_T("---- File Write Result ----\n"));
	_tprintf(_T("Error code : %u \n"), errorCode);
	_tprintf(_T("Transfered bytes len : %u \n"), numOfBytesTransfered);
	_tprintf(_T("The other info : %u \n"), (DWORD)overlapped->hEvent);
}