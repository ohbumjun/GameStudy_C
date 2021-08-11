#pragma once

#include<assert.h>
#include "Queue.h"
#include "Stack.h"
#include "Heap.h" // 최단경로인 애들만 뽑아내기 
#include "List.h" // 최종 경로를 어떻게 갔는지

// 그래프 ?
// 여러가지 노드들이 존재할 때 
// 해당 노드들을 연결하는 형태를 의미한다.

// 그리고 노드들을 연결하는 선들을
// "엣지"라고 한다.

// 그래프는 보통 크게 2가지 방법으로 정의하고 있다
// 1) 방향 (A노드->B노드 가능 , B->A는 x )
// 2) 무방향( A->B, B->A 둘다 가능 )

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
	// 해당 edge에 연결된 노드 
	class CDijkstraNode<T>* m_Node;
	// 비용( 다음 노드로 이동하기 위한 비용 )
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
		// 포인터 배열 
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
// 하나의 node는
// 여러개의 edge를 가질 수 있다
private :
	CDijkstraNode<T>* m_Parent;
	CEdge<T>* m_EdgyArray;
	int m_Size;
	int m_Capacity;
	T m_Data;
	bool m_Visit; // 해당 노드 방문 여부
	// 노드들도 비용 필요
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

		// 추가 edge를 만들어준다
		CEdge<T>* Edge = new CEdge<T>;
		Edge->m_Node = Node;
		Edge->m_Cost = Cost;
		m_EdgyArray[m_Size] = Edge;
		++m_Size;
	}

	void Add(CHeap<CDijkstraNode<T>*>& queue)
	{
		// edge size만큼 반복돌면서
		// edge가 가지고 있는 노드를 방문시도
		for (int i = 0; i < m_Size; i++)
		{
			// 현재 노드의 비용과 Edge의 비용을 더하여
			// Edge에 연결된 ㄴ드의 비용과
			// 비교를 하고
			// 만약 이 비용이 Edge에 연결된 노드의 비용보다
			// 더 작다면
			// 부모노드를 교체해주고
			// 비용을 교체한다 
			if (m_EdgyArray[i]->m_Node->m_Visit)
			{
				m_EdgyArray[i]->m_Node->m_Cost = m_Cost + m_EdgyArray[i]->m_Cost;
				m_EdgyArray[i]->m_Node->m_Parent = this;
			}

			// 방문안한 노드라면 heap에 넣어준다
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
	// 여러개의 노드를 관리할 수 
	// 있어야 한다
public :
	CDijkstra()
	{
	// 동적할당된ㄷ
	// 노드 포인터 배열을 가질 수 있도록 해야함
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
		// 일단 노드 만들기
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

	// 노드에 edge를 추가해준다
	// 여기서는 무방향 
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

		// 하나라도 못찾았다면 걸러내기
		if (!SrcNode || !DestNode)
			return;

		// 서로 추가 
		SrcNode->AddEdge(DestNode, Cost);
		DestNode->AddEdge(SrcNode, Cost);
	}
	bool Find(CList<T>* ResultList, const T& Start, const T& End)
	{
		// 시작점에서 도착점으로 찾아가게끔 하기
		CDijkstraNode<T>* StartNode = FindNode(Start);
		if (!StartNode)
			return false;
		CDijkstraNode<T>* EndNode = FindNode(End);
		if (!EndNode)
			return false;
		// 시작노드,도착노드 정보 찾으면
		// 모든 노드 정보 초기화
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Cost = INT_MAX;
			m_NodeArray[i]->m_Parent = nullptr;
			m_NodeArray[i]->m_Visit = false;
		}
		// 시작노드의 비용을 0으로 처리
		StartNode->m_Cost = 0;

		// 비용의 크기에 따라 정렬해서
		// 사용해야 하므로, 
		// Heap을 이용하여 처리한다
		CHeap<CDijkstraNode<T>*> heap;

		// 정렬을 하는 기준은
		// 무조건 비용 
		heap.SetSortNode(SortNode);

		// 시작노드를 방문상태로 만들고
		// heap에 넣어준다
		StartNode->m_Visit = true;

		// 자동 정렬
		heap.push(StartNode);

		while (!heap.empty())
		{
			CDijkstraNode<T>* Node = heap.top();
			heap.pop();

			// Heap에다가 연결된 노드를 다 넣고
			// Parent를 알아서 설정해주게 될 것이다.
			// 즉, heap을 통해 새로 연결된 노드들의 parent를
			// Node로 세팅해주게 될 것이다. 
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

	// 노드 정렬
	static bool SortNode(CDijkstraNode<T>* Left, CDijkstraNode<T>* Right)
	{
		return Left->m_Cost > Right->m_Cost;
	}
};

