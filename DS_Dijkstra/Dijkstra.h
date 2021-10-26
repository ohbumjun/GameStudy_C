#pragma once
#include<assert.h>
#include"Heap.h"
#include"List.h"


template<typename T>
class CEdge
{
	template<typename T>
	friend class CDijkstraNode;
	template<typename T>
	friend class CDijkstra;
public :
	CEdge() { m_Node = nullptr; }
	~CEdge(){}
private :
	int m_Cost;
	class CDijkstraNode<T>* m_Node;
};

template<typename T>
class CDijkstraNode
{
	template<typename T>
	friend class CDijkstra;
public :
	CDijkstraNode()
	{
		m_Cost = INT_MAX;
		m_Size = 0;
		m_Capacity = 8;
		m_Visit = false;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
	}
	~CDijkstraNode()
	{
		for (int i = 0; i < m_Size; i++)
			delete m_EdgeArray[i];
		delete[] m_EdgeArray;
	}
private :
	int m_Cost;
	int m_Size;
	int m_Capacity;
	bool m_Visit;
	T m_Data;
	CEdge<T>** m_EdgeArray;
	CDijkstraNode<T>* m_Parent;
public :
	void AddEdge(CDijkstraNode<T>* Node, int Cost)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CEdge<T>** Array = new CEdge<T>*[m_Capacity];
			memcpy(Array, m_EdgeArray, sizeof(CEdge<T>*) * m_Size);
			delete[] m_EdgeArray;
			m_EdgeArray = Array;
		}
		CEdge<T>* Edge = new CEdge<T>;
		Edge->m_Node = Node;
		Edge->m_Cost = Cost;
		m_EdgeArray[m_Size] = Edge;
		++m_Size;
	}
	void Add(CHeap<CDijkstraNode<T>*>& heap)
	{
		// 다익스트라 : 아직 방문안한 노드 중에서 가장 거리가 짧은 노드를 방문한다.
		// 그리고 각 점까지의 최소 거리들을 update 해간다
		for (int i = 0; i < m_Size; i++)
		{
			if (m_Cost + m_EdgeArray[i]->m_Cost < m_EdgeArray[i]->m_Node->m_Cost)
			{
				m_EdgeArray[i]->m_Node->m_Cost = m_Cost + m_EdgeArray[i]->m_Cost;
				m_EdgeArray[i]->m_Node->m_Parent = this;
			}
			if (!m_EdgeArray[i]->m_Node->m_Visit)
			{
				m_EdgeArray[i]->m_Node->m_Visit = true;
				heap.push(m_EdgeArray[i]->m_Node);
			}
		}
	}
};

template<typename T>
class CDijkstra
{
public :
	CDijkstra()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_NodeArray = new CDijkstraNode<T>*[m_Capacity];
	}
	~CDijkstra()
	{
		for (int i = 0; i < m_Size; i++)
			delete m_NodeArray[i];
		delete[] m_NodeArray;
	}
public :
	void insert(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CDijkstraNode<T>** Array = new CDijkstraNode<T>*[m_Capacity];
			memcpy(Array, m_NodeArray, sizeof(CDijkstraNode<T>*) * m_Size);
			delete[] m_NodeArray;
			m_NodeArray = Array;
		}
		CDijkstraNode<T>* Node = new CDijkstraNode<T>;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
		++m_Size;
	}
	bool Find(CList<T>* ResultList, const T& Start, const T& End)
	{
		CDijkstraNode<T>* StartNode = FindNode(Start);
		if (!StartNode) return false;
		CDijkstraNode<T>* EndNode   = FindNode(End);
		if (!EndNode) return false;

		// 초기화
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Visit = false;
			m_NodeArray[i]->m_Cost = INT_MAX;
			m_NodeArray[i]->m_Parent = nullptr;
		}

		CHeap<CDijkstraNode<T>*> heap;
		StartNode->m_Visit = true;
		StartNode->m_Cost = 0;
		heap.push(StartNode);
		while (!heap.empty())
		{
			CDijkstraNode<T>* Node = heap.top();
			heap.pop();
			Node->Add(heap);
		}
		CDijkstraNode<T>* Node = EndNode;
		while (Node)
		{
			ResultList->push_front(Node->m_Data);
			Node = Node->m_Parent;
		}
		return true;
	}
	CDijkstraNode<T>* FindNode(const T& data)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == data) return m_NodeArray[i];
		}
		return nullptr;
	}
	void AddEdge(const T& Src, const T& Dest, int Cost)
	{
		CDijkstraNode<T>* SrcNode  = nullptr;
		CDijkstraNode<T>* DestNode = nullptr;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
				SrcNode = m_NodeArray[i];
			if (m_NodeArray[i]->m_Data == Dest)
				DestNode = m_NodeArray[i];
			if (SrcNode && DestNode) break;
		}
		if (!SrcNode || !DestNode) return;
		SrcNode->AddEdge(DestNode, Cost);
		DestNode->AddEdge(SrcNode, Cost);
	}

private :
	int m_Size;
	int m_Capacity;
	CDijkstraNode<T>** m_NodeArray;
};