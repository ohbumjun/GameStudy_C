#pragma once 
#include "List.h"
#include "Heap.h"

template<typename T>
class CEdge;
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
	CEdge<T>** m_EdgeArray;
	CDijkstraNode<T>* m_Parent;
	T m_Data;
	bool m_Visit;
	int m_Size;
	int m_Capacity;
	int m_Cost;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void AddNode(CDijkstraNode<T>* Node, int Cost)
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
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			if (m_Cost + m_EdgeArray[i]->m_Cost < m_EdgeArray[i]->m_Node->m_Cost)
			{
				m_EdgeArray[i]->m_Node->m_Cost   = m_Cost + m_EdgeArray[i]->m_Cost;
				m_EdgeArray[i]->m_Node->m_Visit  = true;
				m_EdgeArray[i]->m_Node->m_Parent = this;
				heap.push(m_EdgeArray[i]->m_Node);
			}
		}
	}
};

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
private :
	CDijkstraNode<T>* m_Node;
	int m_Cost;
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
private :
	class CDijkstraNode<T>** m_NodeArray;
	int m_Size;
	int m_Capacity;
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
	void AddEdge(const T& St, const T& End, int Cost)
	{
		CDijkstraNode<T>* StNode = nullptr;
		CDijkstraNode<T>* EdNode = nullptr;

		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == St)
				StNode = m_NodeArray[i];
			if (m_NodeArray[i]->m_Data == End)
				EdNode = m_NodeArray[i];
			if (StNode && EdNode) break;
		}
		if (!StNode || !EdNode) return;
		StNode->AddNode(EdNode, Cost);
		EdNode->AddNode(StNode, Cost);
	}
	bool Find(CList<const char*>* ResultList, const T& St, const T& Ed)
	{
		CDijkstraNode<T>* StNode = FindNode(St);
		CDijkstraNode<T>* EdNode = FindNode(Ed);

		if (!StNode || !EdNode) return false;

		for (int i = 0; i < m_Size; i++)
		{
			m_NodeArray[i]->m_Visit  = false;
			m_NodeArray[i]->m_Cost   = INT_MAX;
			m_NodeArray[i]->m_Parent = nullptr;
		}

		CHeap<CDijkstraNode<T>*> heap;
		StNode->m_Visit = true;
		StNode->m_Cost = 0;
		heap.push(StNode);
		while (!heap.empty())
		{
			CDijkstraNode<T>* Node = heap.top();
			heap.pop();
			Node->Add(heap);
		}

		CDijkstraNode<T>* Node = EdNode;
		while (Node)
		{
			ResultList->push_back(Node->m_Data);
			Node = Node->m_Parent;
		}
		return true;
	}
	CDijkstraNode<T>* FindNode(const T& Data)
	{
		for (int i = 0; i < m_Size; i++)
			if (m_NodeArray[i]->m_Data == Data) return m_NodeArray[i];
		return nullptr;
	}
};