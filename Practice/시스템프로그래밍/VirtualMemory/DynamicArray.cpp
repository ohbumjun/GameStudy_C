// Dynamic Array Design
// -> �Ϲ����� �迭 ó�� �Ѽ����� �迭 ũ�⸸ŭ
//      ���� �޸𸮰� �Ҵ�Ǵ� ���� �ƴ϶�
//      ��뷮�� ������ ���� ���� �޸𸮿� �Ҵ�Ǵ�
//      , ��, �迭�� ũ�Ⱑ ���������� �����ϴ� �迭

/*
1. �ý����� pageSize, allocGranularity �� ���´�.

2. �޸𸮸� �����Ѵ�.

3. �ʿ��� ��ŭ�� �޸𸮸�, ���� �޸𸮿� commit (�Ҵ�)

4. �޸� ��ȯ
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#define MAX_PAGE 10 // �Ҵ��� �ִ� ������ ����

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;
int PageFaultExceptionFilter(DWORD);

int _tmain(int argc, TCHAR* argv[])
{
	LPVOID baseAddr;
	int* lpPtr;
	SYSTEM_INFO sSysInfo;

	GetSystemInfo(&sSysInfo);
	pageSize = sSysInfo.dwPageSize;

	// MAX_PAGE ������ŭ Reserve
	baseAddr = VirtualAlloc(
		// - ���� �� �Ҵ��Ϸ��� �޸� ���� �ּ�
		// - �Ϲ������� NULL ����
		//   NULL �� �Ҵ�Ǹ�, �Ҵ��ϰ��� �ϴ� ũ�⿡ ���缭
		//   �޸� ��ġ�� ���Ƿ� �����Ѵ�.
		// - ������, Reserve ������ �������� Commit ���·� ������ ������
		//   �ش� �������� ���� �ּҸ� �����ؾ� �Ѵ�.
		NULL,                          // ���� �ּ� �Ҵ�

		// - �Ҵ��ϰ��� �ϴ� ũ�� ������ ����Ʈ ������ ����
		MAX_PAGE * pageSize, // ���� �޸� ũ��

		// - �޸� �Ҵ� Ÿ��
		MEM_RESERVE,            // RESERVE

		// - �������� ���ٹ�Ŀ� ������ �δ� �뵵
		// - Reserve ���¿� �� ������ ������ ������� �ʴ� PAGE_NOACCESS
		// - Commit ���·� ������ ������,  �б� ���� ��� ����ϴ� PAGE_READWRITE
		PAGE_NOACCESS         // NO ACCESS
	);

	if (baseAddr == NULL)
		_tprintf(_T("Virtual Alloc reserve failed"));

	// �迭�� ���� ������ ���� �ּ� �ο�
	lpPtr = (int*)baseAddr;
	nextPageAddr = (int*)baseAddr;

	// page fault �߻� �� ���� �߻�
	for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	{
		__try
		{
			lpPtr[i] = i;
		}
		__except (PageFaultExceptionFilter(GetExceptionCode()))
		{
			// ����ó�� ���� �߻��� ����
			ExitProcess(GetLastError());
		}
	}

	//for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	//{
	//	_tprintf(_T("%d \n"), lpPtr[i]);
	//}

	BOOL isSuccess = VirtualFree(
		baseAddr,         // ������ �޸��� ���� ����
		0,					   // ������ �޸� ũ�⸦ ����Ʈ ������ ���� 


		// - MEM_RELEASE ������ ���, FREE ���°� �ȴ�.
		//   MEM_RELEASE ���޽� 2��° ���ڴ� 0�� �Ǿ�� �Ѵ�.
		//   MEM_RELEASE ���޽� lpAddr �� ����, VirtualAlloc �Լ� ȣ���� ���� ����� �޸� ���� �������߸� �Ѵ�.
		// - MEM_DECOMMIT ������ ���, RESERVE ���·�
		//   2�� ���, ������ �޸𸮰� �Ҵ�Ǿ� �ִٸ�, �ش� �޸� ��ȯ��.
		MEM_RELEASE // MEM_DECOMMIT Ȥ�� MEM_RELESE �� �ϳ� ����
	);

	if (isSuccess)
		_tprintf(_T("Release succeeded"));
	else
		_tprintf(_T("Release failed"));

	return 0;
};

int PageFaultExceptionFilter(DWORD exptCode)
{
	// ���� ������ page fault �� �ƴ϶��
	if (exptCode != EXCEPTION_ACCESS_VIOLATION)
	{
		_tprintf(_T("Exception Code = %d \n"), exptCode);

		// __except �Լ� ����
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// ���� �޸𸮿� �Ҵ��� �̷������ �ʾƼ�
	// �߻��� �����̹Ƿ�, ������ �� �������� COMMIT ���·� 
	_tprintf(_T("Exception is a page fault \n"));

	if (pageCnt >= MAX_PAGE)
	{
		_tprintf(_T("Exception : Out Of Page \n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}

	LPVOID lpvResult = VirtualAlloc(
		(LPVOID)nextPageAddr,
		pageSize,
		MEM_COMMIT,
		PAGE_READWRITE
	);

	if (lpvResult == NULL)
	{
		_tprintf(_T("Virtual Alloc Failed \n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		_tprintf(_T("Allocating another page \n"));
	}

	pageCnt++;

	// COMMIT ���¿� �ִ� �������� ��ġ ���� ����
	nextPageAddr += pageSize / sizeof(int);

	// page fault�� �߻��� �������� ���� ���
	// (���ܰ� �߻��� ��ġ�� ���ư��� ���� �̾ ���̴�.)
	return EXCEPTION_CONTINUE_EXECUTION;
}