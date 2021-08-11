#pragma once

#include<assert.h>
#include "Queue.h"
#include "Stack.h"
#include "Heap.h" // �ִܰ���� �ֵ鸸 �̾Ƴ��� 
#include "List.h" // ���� ��θ� ��� ������

// �׷��� ?
// �������� ������ ������ �� 
// �ش� ������ �����ϴ� ���¸� �ǹ��Ѵ�.

// �׸��� ������ �����ϴ� ������
// "����"��� �Ѵ�.

// �׷����� ���� ũ�� 2���� ������� �����ϰ� �ִ�
// 1) ���� (A���->B��� ���� , B->A�� x )
// 2) ������( A->B, B->A �Ѵ� ���� )

template<typename T>
class CEdge
{
	template<typename T>
	friend class CDijkstra;
	template<typename T>
	friend class CDijkstraNode;
public :
	CEdge()
	{
		m_Node = nullptr;
	}
	~CEdge()
	{

	}
private :
	// �ش� edge�� ����� ��� 
	class CDijkstraNode<T>* m_Node;
	// ���( ���� ���� �̵��ϱ� ���� ��� )
	int m_Cost;
};

template<typename T>
class CDijkstraNode
{
	template<typename T>
	friend class CDijkstra;
private :
	CDijkstraNode()
	{
		m_Size = 0;
		m_Capacity = 1;
		// ������ �迭 
		m_EdgeArray = new CEdge<T>*[m_Capacity];
		m_Visit = false;
		m_Cost = INT_MAX;
		m_Parent = nullptr;
	}
	~CDijkstraNode()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete[] m_EdgyArray[i];
		}
		delete[] m_EdgyArray;
	}
// �ϳ��� node��
// �������� edge�� ���� �� �ִ�
private :
	CDijkstraNode<T>* m_Parent;
	CEdge<T>* m_EdgyArray;
	int m_Size;
	int m_Capacity;
	T m_Data;
	bool m_Visit; // �ش� ��� �湮 ����
	// ���鵵 ��� �ʿ�
	int m_Cost;

private :
	void AddEdge(CDijkstraNode<T> Node, int Cost)
	{
		if (m_Capacity == m_Size)
		{
			m_Capacity *= 2;
			CEdge<T>** Array = new CEdge<T>*[m_Capacity];

			memcpy(Array, m_EdgyArray, sizeof(CEdge<T>*) *m_Size);
			delete[] m_EdgyArray;
			m_EdgyArray = Array;
		}

		// �߰� edge�� ������ش�
		CEdge<T>* Edge = new CEdge<T>;
		Edge->m_Node = Node;
		Edge->m_Cost = Cost;
		m_EdgyArray[m_Size] = Edge;
		++m_Size;
	}

	void Add(CHeap<CDijkstraNode<T>*>& queue)
	{
		// edge size��ŭ �ݺ����鼭
		// edge�� ������ �ִ� ��带 �湮�õ�
		for (int i = 0; i < m_Size; i++)
		{
			// ���� ����� ���� Edge�� ����� ���Ͽ�
			// Edge�� ����� ������ ����
			// �񱳸� �ϰ�
			// ���� �� ����� Edge�� ����� ����� ��뺸��
			// �� �۴ٸ�
			// �θ��带 ��ü���ְ�
			// ����� ��ü�Ѵ� 
			if (m_EdgyArray[i]->m_Node->m_Visit)
			{
				m_EdgyArray[i]->m_Node->m_Cost = m_Cost + m_EdgyArray[i]->m_Cost;
				m_EdgyArray[i]->m_Node->m_Parent = this;
			}

			// �湮���� ����� heap�� �־��ش�
			if (!m_EdgyArray[i]->m_Node->m_Visit)
			{
				heap.push();
			}

		}
	}
	void Add(CStack<CDijkstraNode<T>*>& queue)

};

template<typename T>
class CDijkstra
{
	// �������� ��带 ������ �� 
	// �־�� �Ѵ�
public :
	CDijkstra()
	{
	// �����Ҵ�Ȥ�
	// ��� ������ �迭�� ���� �� �ֵ��� �ؾ���
		m_Size = 0;
		m_Capacity = 4;
		m_NodeArray = new CDijkstraNode<T>*[m_Capacity];
	
	}
	~CDijkstra()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_NodeArray[i];
		}
		delete[] m_NodeArray;
	}
private :
	CDijkstraNode<T>** m_NodeArray;
	int m_Size;
	int m_Capacity;
public :
	void insert(const T& data)
	{
		// �ϴ� ��� �����
		if (m_Capacity == m_Size)
		{
			m_Capacity *= 2;
			
			CDijkstraNode<T>** Array = new CDijkstraNode<T>*[m_Capacity];
			
			memcpy(Array, m_NodeArray, sizeof(CDijkstraNode<T>*) * m_Size);

			delete []m_NodeArray;
			m_NodeArray = Array;
		}

		CDijkstraNode<T>* Node = new CDijkstraNode<T>;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
		++m_Size;
	}

	// ��忡 edge�� �߰����ش�
	// ���⼭�� ������ 
	void AddEdge(const T& Src, const T& Dest int Cost)
	{
		// Src  : 
		// Dest :
		CDijkstraNode<T>* SrcNode = nullptr;
		CDijkstraNode<T>* DestNode = nullptr;
	
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
			{
				SrcNode = m_NodeArray[i];
			}
			else if (m_NodeArray[i]->m_Data == Dest)
			{
				DestNode = m_NodeArray[i];
			}
			if (SrcNode && DestNode)
				break;
		}

		// �ϳ��� ��ã�Ҵٸ� �ɷ�����
		if (!SrcNode || !DestNode)
			return;

		// ���� �߰� 
		SrcNode->AddEdge(DestNode, Cost);
		DestNode->AddEdge(SrcNode, Cost);
	}
	bool Find(CList<T>* ResultList, const T& Start, const T& End)
	{
		// ���������� ���������� ã�ư��Բ� �ϱ�
		CDijkstraNode<T>* StartNode = FindNode(Start);
		if (!StartNode)
			return false;
		CDijkstraNode<T>* EndNode = FindNode(End);
		if (!EndNode)
			return false;
		// ���۳��,������� ���� ã����
		// ��� ��� ���� �ʱ�ȭ
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Cost = INT_MAX;
			m_NodeArray[i]->m_Parent = nullptr;
			m_NodeArray[i]->m_Visit = false;
		}
		// ���۳���� ����� 0���� ó��
		StartNode->m_Cost = 0;

		// ����� ũ�⿡ ���� �����ؼ�
		// ����ؾ� �ϹǷ�, 
		// Heap�� �̿��Ͽ� ó���Ѵ�
		CHeap<CDijkstraNode<T>*> heap;

		// ������ �ϴ� ������
		// ������ ��� 
		heap.SetSortNode(SortNode);

		// ���۳�带 �湮���·� �����
		// heap�� �־��ش�
		StartNode->m_Visit = true;

		// �ڵ� ����
		heap.push(StartNode);

		while (!heap.empty())
		{
			CDijkstraNode<T>* Node = heap.top();
			heap.pop();

			// Heap���ٰ� ����� ��带 �� �ְ�
			// Parent�� �˾Ƽ� �������ְ� �� ���̴�.
			// ��, heap�� ���� ���� ����� ������ parent��
			// Node�� �������ְ� �� ���̴�. 
			Node->Add(heap);
		}

		CDijkstraNode<T>* Node = EndNode;
		
		while (Node)
		{
			ResultList->push_front(Node->m_Data);
			Node = Node->m_Parent;
		}

	}

private :
	CDijkstraNode<T>* FindNode(const T& data)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == data)
				return m_NodeArray[i];
		}
		return nullptr;
	}

	// ��� ����
	static bool SortNode(CDijkstraNode<T>* Left, CDijkstraNode<T>* Right)
	{
		return Left->m_Cost > Right->m_Cost;
	}
};

