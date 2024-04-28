#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

typedef int ListElementDataType;

typedef struct _node
{
	ListElementDataType data;
	struct _node* next;
} node;

node* head;
node* tail;

HANDLE hHeap = 0;

void InitListHeap()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	UINT pageSize = sysInfo.dwPageSize;

	hHeap = HeapCreate(
		HEAP_NO_SERIALIZE,
		pageSize * 10,
		pageSize * 100
	);
};

void InitList(void)
{
	InitListHeap();

	// head = (node*)malloc(sizeof(node))
	head = (node*)HeapAlloc(
		hHeap, HEAP_NO_SERIALIZE,
		sizeof(node)
	);

	// tail = (node*)malloc(sizeof(node))
	tail = (node*)HeapAlloc(
		hHeap, HEAP_NO_SERIALIZE,
		sizeof(node)
	);

	head->next = tail;
	tail->next = tail;
};

// �Էµ� data �� ������ ���� ���ϴ� ù���� ��� ����
int DeleteNode(int data)
{
	node* nd = head;
	node* ndNext = nd->next;

	while (ndNext->data != data && ndNext != tail)
	{
		nd = nd->next;
		ndNext = nd->next;
	};

	// ������ ���� ���ϴ� ��带 �߰��ߴٸ�
	if (ndNext != tail)
	{
		nd->next = ndNext->next;

		// free(ndNext)
		HeapFree(hHeap, HEAP_NO_SERIALIZE, ndNext);

		return 1;
	}
	else
		return 0;
};

// �Էµ� data �� ����Ʈ�� �����ϵ�
// �����ؼ� ����
void OrderedInsert(int data)
{

}