#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// 1�ܰ� ���� ����
	// - ���� �ڵ��� ���߸� �Ѵ�.
	HANDLE hFile =
		CreateFile(
			_T("data.dat"),
			// �ͱ�, ���� ��� �����ϵ��� ���� ���� ����
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	if (hFile == INVALID_HANDLE_VALUE)
		_tprintf(_T("Could not open file\n"));

	TCHAR fileData[] = _T("Best test string~ ^^");
	DWORD numOfByteWritten = 0;

	WriteFile(
		hFile,
		fileData,
		sizeof(fileData),
		&numOfByteWritten,
		NULL
	);
	
	// 2�ܰ� : ���� ���� ������Ʈ ����
	// - �޸𸮿� ������ ���� ������ ��� �ִ�
	//   Ŀ�� ������Ʈ�� �����ϴ� ���̴�.
	HANDLE hMapFile =
		CreateFileMapping(
			hFile, 
			// Ŀ�� ������Ʈ �����ϴ� �ٸ� �Լ� ���� ���� �Ӽ� ����
			NULL,
			// ���ϰ� ����� �޸��� ���� ������ �����Ѵ�.
			// (�б⸸ ����)
			PAGE_READONLY,
			// ������ �޸� �ִ� ũ���� ���� 4����Ʈ
			// ��뷮 ������ �ƴ϶�� 0���� ���
			0, 
			// ���� 4����Ʈ
			// 0 �� �����ϸ�, ù��° ���ڷ� ���޵�
			// �ڵ��� ���� ũ��� �����ȴ�.
			0, 
			// ���� ���� ������Ʈ�� �̸�
			NULL
		);

	if (hMapFile == NULL)
		_tprintf(_T("Could not create map of file\n"));

	// 3�ܰ� : �޸𸮿� ����
	// - ������ ���� �޸𸮿� ������ �����Ѵ�.
	TCHAR* pWrite =
		(TCHAR*)MapViewOfFile(
			hMapFile,
			// ����� �޸��� ���� ����
			// CreateFileMapping �Լ��� 3��° ���� ������
			// PAGE_READWRITE ���
			// �б�� ���� �� �ϳ� Ȥ�� �Ѵ� 
			// PAGE_READ ��� ������ �б� ����
			FILE_MAP_READ,
			// ������ ���� ���θ� �޸𸮿� �����ؾ� �ϴ� ����
			// �ƴϴ�. ������ �Ϻ� ������ �����ؼ� �����ϴ� �͵�
			// �����ϴ�
			// �޸𸮿� ������ ������ ������(���۹���) ����
			// ���� 4����Ʈ ����
			0,
			// ���� 4����Ʈ
			0,
			// �޸𸮿� ������ ���� ũ�⸦ ����Ʈ ������ ����
			// ������ �������� ������ ����Ʈ ��ŭ �޸𸮿� ����
			// 0�� �����ϸ�, ������ �����¿�������
			// ���� ������ �޸𸮿� ����
			0
		);

	if (pWrite == NULL)
		_tprintf(_T("Could not map view of file\n"));
	
	_tprintf(_T("String in file : %s\n"), pWrite);

	// �ش� �Լ� ȣ���� ����
	// �Ҵ�Ǿ��� ���� �޸𸮿��� ������ ���´�.
	// �̰��� ���� ���� ����� �Ѵ�.
	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return 0;
}