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
	node* nd = head;
	node* ndNext = nd->next;
	node* newNode;

	while (ndNext->data <= data && ndNext != tail)
	{
		nd = nd->next;
		ndNext = nd->next;
	}

	// newNode = (node*)malloc(sizeof(node))
	newNode = (node*)HeapAlloc(
		hHeap,
		HEAP_NO_SERIALIZE,
		sizeof(node)
	);

	newNode->data = data;
	nd->next = newNode;
	newNode->next = ndNext;
};

// ����Ʈ�� ���ĵ� ��� ������ ���
void PrintAllList()
{
	node* startNode = head->next;

	while (startNode != tail)
	{
		printf("%-4d", startNode->data);
		startNode = startNode->next;
	}
	printf("\n\n");
}

// ����Ʈ�� ����� ��� ������ ����
void DeleteAll(void)
{
	//	node* nd = head->next;
	//	node* delNode;
	//	while (nd != tail)
	//	{
	//		delNode = nd;
	//		nd = nd->next;
	//		free(delNode);
	//	}
	//	head->next = tail;

	// ���� ������ �ڵ�� ��ü
	HeapDestroy(hHeap);
	InitList();
};

int main(int argc, char* argv[])
{
	InitList();

	OrderedInsert(1);
	OrderedInsert(2);
	OrderedInsert(3);
	OrderedInsert(4);
	OrderedInsert(5);
	PrintAllList();

	DeleteNode(2);
	DeleteNode(4);
	PrintAllList();

	printf("��� ����Ʈ ����\n");
	DeleteAll();

	return 0;
}