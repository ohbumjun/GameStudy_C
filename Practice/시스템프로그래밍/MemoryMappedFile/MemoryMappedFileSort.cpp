/*
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// ���Ͽ� �����͸� �����ϰ�
// ����� �����͸� �����ϱ�

void SortIntData(int* pSortArr, int num);

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

	// 2�ܰ� : ���� ���� ������Ʈ ����
	// - �޸𸮿� ������ ���� ������ ��� �ִ�
	//   Ŀ�� ������Ʈ�� �����ϴ� ���̴�.
	HANDLE hMapFile =
		CreateFileMapping(
			hFile,
			// Ŀ�� ������Ʈ �����ϴ� �ٸ� �Լ� ���� ���� �Ӽ� ����
			NULL,
			// ���ϰ� ����� �޸��� ���� ������ �����Ѵ�.
			// (�б�,���� ����)
			PAGE_READWRITE,
			// ���� ���� �ִ� �޸� ũ��
			// ���� 4����Ʈ
			// ��뷮 ������ �ƴ϶�� 0���� ���
			0,
			// ���� ���� �ִ� �޸� ũ��
			// ���� 4����Ʈ
			// 0 �� �����ϸ�, ù��° ���ڷ� ���޵�
			// �ڵ��� ���� ũ��� �����ȴ�.
			1024 * 10,
			// ���� ���� ������Ʈ�� �̸�
			NULL
		);

	// 1024 * 10 --> MMF �� ����Ǵ� �޸� ũ���
	// ���⼭ �����ϴ� �� �̻����� �������� �ʴ´�.
	// 0 �� �����Ѵٸ�, ���� ������ ũ�Ⱑ 0�̹Ƿ�
	// MMF �ִ� ũ��� 0���� �����Ǿ� ������.
	// �޸𸮿� ���� ������ �ִ� ũ�Ⱑ 0����Ʈ ��� ����
	// �ɰ��� �����̴�.
	// ���� ��������� ���� ������ �ִ� ũ�⸦
	// ��������߸� �Ѵ�.

	// ��, ������ ũ�Ⱑ ������ 10K ����Ʈ���� �۴ٸ�
	// 10K ����Ʈ�� ���ϰ� �޸� ���ῡ ����
	// ��û�� ó���� �� �����Ƿ�
	// "MMF �ִ� �޸� ũ�⸸ŭ ���� ũ�⵵ �����Ѵ�."

	if (hMapFile == NULL)
		_tprintf(_T("Could not create map of file\n"));

	// 3�ܰ� : �޸𸮿� ����
	// - ������ ���� �޸𸮿� ������ �����Ѵ�.
	int* pWrite =
		(int*)MapViewOfFile(
			hMapFile,
			// ����� �޸��� ���� ����
			// CreateFileMapping �Լ��� 3��° ���� ������
			// PAGE_READWRITE ���
			// �б�� ���� �� �ϳ� Ȥ�� �Ѵ� 
			// PAGE_READ ��� ������ �б� ����
			FILE_MAP_ALL_ACCESS,
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

	pWrite[0] = 1;
	pWrite[1] = 3;
	pWrite[2] = 0;
	pWrite[3] = 2;
	pWrite[4] = 4;
	pWrite[5] = 5;
	pWrite[6] = 8;
	pWrite[7] = 6;
	pWrite[8] = 7;

	SortIntData(pWrite, 9);

	_tprintf(_T("%d, %d, %d\n"), pWrite[0], pWrite[1], pWrite[2]);
	_tprintf(_T("%d, %d, %d\n"), pWrite[3], pWrite[4], pWrite[5]);
	_tprintf(_T("%d, %d, %d\n"), pWrite[6], pWrite[7], pWrite[8]);

	// �ش� �Լ� ȣ���� ����
	// �Ҵ�Ǿ��� ���� �޸𸮿��� ������ ���´�.
	// �̰��� ���� ���� ����� �Ѵ�.
	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return 0;
};

void SortIntData(int* pSortArr, int num)
{
	// bubble sort
	int temp;

	for (int i = 0; i < num - 1; ++i)
	{
		for (int j = 1; j < num; ++j)
		{
			if (pSortArr[j - 1] > pSortArr[j])
			{
				temp = pSortArr[j - 1];
				pSortArr[j - 1] = pSortArr[j];
				pSortArr[j] = temp;
			}
		}
	}
}
*/