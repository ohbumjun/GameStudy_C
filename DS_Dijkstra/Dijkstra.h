#pragma once
#include"Heap.h"
#include"List.h"
#include"Queue.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CDijkstraNode;
	template<typename T>
	friend class CDijkstra;
public :
	CEdge()
	{
		m_Node = nullptr;
	}
	~CEdge()
	{

	}
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
		m_Size = 0;
		m_Capacity = 8;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
		m_Visit = false;
		m_Cost = INT_MAX;
		m_Parent = nullptr;
	}
	~CDijkstraNode()
	{
		for (int i = 0; i < m_Size; i++)
			delete m_EdgeArray[i];
		delete[] m_EdgeArray;
	}
private :
	int m_Size;
	int m_Capacity;
	CEdge<T>** m_EdgeArray;
	bool m_Visit;
	T m_Data;
	int m_Cost;
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
		for (int i = 0; i < m_Size; i++)
		{
			if (m_Cost + m_EdgeArray[i]->m_Cost < m_EdgeArray[i]->m_Node->m_Cost)
			{
				m_EdgeArray[i]->m_Node->m_Cost = m_Cost + m_EdgeArray[i]->m_Cost;
				m_EdgeArray[i]->m_Node->m_Parent = this;
			}
			if (!m_EdgeArray[i]->m_Node->m_Visit)
			{
				heap.push(m_EdgeArray[i]->m_Node);
				m_EdgeArray[i]->m_Node->m_Visit = true;
			}
		}
	}
};


template<typename T>
class CDijkstra
{
public:
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
private:
	int m_Size;
	int m_Capacity;
	CDijkstraNode<T>** m_NodeArray;
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
	CDijkstraNode<T>* FindNode(const T& data)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == data) return m_NodeArray[i];
		}
		return nullptr;
	}
	bool Find(CList<T>* ResultList, const T& Src, const T& Dest)
	{
		// 시작, 끝점 설정
		CDijkstraNode<T>* StartNode = FindNode(Src);
		if (!StartNode) return false;
		CDijkstraNode<T>* EndNode = FindNode(Dest);
		if (!EndNode) return false;

		// 초기화
		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Visit = false;
			m_NodeArray[i]->m_Parent = nullptr;
			m_NodeArray[i]->m_Cost = INT_MAX;
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

};