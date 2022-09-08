#include <stdio.h>
#include <tchar.h>
#include <windows.h>

TCHAR strData[] =
_T("A\n")
_T("B\n")
_T("C\n")
_T("D\n")
_T("E\n");

// �Ϸ��ƾ I/O �Լ� ȣ�� ���
VOID WINAPI FILEIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);


int _tmain(int argc, TCHAR* argv[])
{
	TCHAR fileName[] = _T("data.txt");

	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_WRITE,
		0, CREATE_ALWAYS, 
		FILE_FLAG_OVERLAPPED, // ��ø I/O �� Ȯ�� 
		0
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File creation fault !\n"));
		return -1;
	}

	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));

	// �Ϸ� ��ƾ I/O �� ���, ��ø I/O �� �޸�
	// �̺�Ʈ ������Ʈ�� �ʿ����� �ʴ�.
	overlappedInst.hEvent = (HANDLE)1234;

	WriteFileEx(hFile, // ������ ���� ���
		strData, // ������ ������ ���� ���� �ּ�
		sizeof(strData),  // �о���� �������� �ִ� ũ��
		&overlappedInst,  
		FILEIOCompletionRoutine); // �Է� ���� �Ϸ�� �ڵ����� ȣ��Ǵ� �Ϸ��ƾ

	// ù��° ���� : �������� ������ ���ߴµ� ����ϴ� ��
	// �ι�° ���� : FALSE �� �Ǹ�, Sleep �Լ��� ����
	// TRUE ���, �ش� �Լ��� ȣ���� �����带 �˸� ������ ���·� ����
	// ��, ȣ��Ǿ���� �Ϸ� ��ƾ�� ȣ���� �̾����� ���̰�
	// �Ϸ� ��ƾ ���� �Ŀ��� SleepEx �Լ��� ��ȯ�ϰ� �ȴ�.
	SleepEx(INFINITE, TRUE);
		
	CloseHandle(hFile);
	return 1;
};


VOID WINAPI FILEIOCompletionRoutine(DWORD errorCode, 
	DWORD numOfBytesTransfered, 
	LPOVERLAPPED overlapped)
{
	// errorCode : ������ GetLastError �Լ��� ȣ���� �ʿ䰡 ����.
	// �ش� ���� 0 �̶�� ����, ������ �߻����� �ʾ����� �ǹ�
	_tprintf(_T("---- File Write Result ----\n"));
	_tprintf(_T("Error code : %u \n"), errorCode);
	_tprintf(_T("Transfered bytes len : %u \n"), numOfBytesTransfered);
	_tprintf(_T("The other info : %u \n"), (DWORD)overlapped->hEvent);
}