#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);
	UINT pageSize = sysInfo.dwPageSize;

	// 1. �� ����
	HANDLE hHeap = HeapCreate(
		// - �����Ǵ� ���� Ư�� �ο�
		// - HEAP_GENERATE_EXCEPTION : ���� �߻��� NULL ��ȯ ���� �ʰ�
		//                                                  ���� �߻�
		// - HEAP_NO_SERIALIZE             : ������ ���� �޸� �Ҵ� ��
		//                                                 ����ȭ ó�� ���� �ʰ� �ȴ� (����ȭ�������� �����ο���)
		//    ������ ����, ������ ���� �Ҵ��� ��� ���
		HEAP_NO_SERIALIZE,

		// - 3��° ���ڷ� ������ �޸� �߿���, �ʱ⿡ �Ҵ��� �޸� ũ��
		// - �ش� ������ ����ŭ Commit ���°� �ȴ�.
		pageSize * 10,

		// - �����Ǵ� ���� ũ��
		// - �����ϴ� ũ�⿡ �ش��ϴ� ������ �� ��ŭ Reserve ���°� �ȴ�.
		// - 0 �� ���޵� ��� ���� ���� ������ �޸𸮰� �ȴ�. 
		//   ��, 0�� ���޵Ǹ�, �޸𸮰� ����ϴ� �ѵ� ������ ���� ũ�� ����
		pageSize * 100
	);	

	// 2. �޸� �Ҵ�
	int* p = (int*)HeapAlloc(
		hHeap, 

		// - HEAP_GENERATE_EXCEPTION : ���� �߻��� NULL ��ȯ ���� �ʰ�
		//                                                  ���� �߻�
		// - HEAP_NO_SERIALIZE             : ������ ���� �޸� �Ҵ� ��
		//                                                 ����ȭ ó�� ���� �ʰ� �ȴ� (����ȭ�������� �����ο���)
		//    ������ ����, ������ ���� �Ҵ��� ��� ���
		//    ��, HeapCreate �ÿ� �����ߴٸ�, �ߺ� ���� �ʿ� X
		// - HEAP_ZERO_MEMORY : �Ҵ�� �޸𸮴� 0 ���� �ʱ�ȭ
		0, 

		// - �Ҵ��ϰ��� �ϴ� �޸� ũ��
		sizeof(int) * 10);

	// 3. �޸� Ȱ��
	for (int i = 0; i < 10; ++i)
		p[i] = i;

	// 4. �޸� ����
	HeapFree(
		hHeap,

		// HEAP_NO_SERIALIZE �� �� �� �ִ�.
		// - HeapCreate �ÿ� �����ߴٸ�, �ߺ� ���� �ʿ� X
		0, 

		// ������ �޸� ���� �ּ�
		p
	);

	// 5. �� �Ҹ�
	HeapDestroy(hHeap);

	// ����Ʈ ���� ���� �� ����ϴ� �Լ�
	HANDLE hDefaultHeap = GetProcessHeap();

	// �Ʒ��� ������, malloc, free �Լ��� ����
	// ����Ʈ ���� �޸𸮸� �Ҵ��ϰ� ��ȯ�ϴ� �Ͱ� ����
	TCHAR* pDefault = (TCHAR*)HeapAlloc(
		hDefaultHeap,
		HEAP_NO_SERIALIZE,
		sizeof(TCHAR) * 30
	);

	// �׷���, �ش� �Լ��� (malloc, free) �� ���� ?
	// - �߰����� ��Ʈ���� �����ϴٴ� ���̴�.
	// - HEAP_NO_SERIALIZE : ����ȭ ó���� ���� �ʴ� ��
	// - ������, �������� �� ������ �Ҵ��ϱ� ������ !

	_tcscpy(pDefault, _T("Default Heap!"));
	_tprintf(_T("%s \n"), pDefault);

	HeapFree(hDefaultHeap, HEAP_NO_SERIALIZE, pDefault);

	return 1;
}