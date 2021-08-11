#pragma once
#include<assert.h>
#include "Queue.h"
#include "CDijkstra.h"
#include "List.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CDijkstra;
	template<typename T>
	friend class CDijkstraNode;
private :
	CEdge() : m_Node(nullptr){}
	~CEdge(){}
private :
	class CDijkstraNode<T>* m_Node;
	int m_Cost;
};

template<typename T>
class CDijkstraNode
{
	template<typename T>
	friend class CDijstra;
private :
	CDijkstraNode()
	{

	}
	~CDijkstraNode()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_EdgeArray[i];
		}
		delete[] m_EdgeArray;
	}
private :
	int m_Size;
	int m_Capacity;
	T** m_EdgeArray;
	bool m_Visit;
	int m_Cost;
	CDijkstraNode<T>* m_Parent;

private :
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
		CEdge<T> Edge = new CEdge<T>;
		Edge->m_Cost = Cost;
		Edge->m_Node = Node;
		m_EdgeArray[m_Size] = Edge;
		++m_Size;
	}
	
	void Add(CHeap<CDijkstraNode<T>*>& heap)
	{
		for (int i = 0; i < m_Size; i++)
		{
			// 현재노드의 비용과
			// Edge의 비용을 더하여
			// Edge에 연결된 노드의 비용과 비교를 하고
			// 만약 이 비용이 Edge에 연결된
			// 노드의 비용보다 더 작다면
			// 부모노드를 교체해주고, 비용을 교체한다
			if (m_Cost + m_EdgeArray[i]->m_Cost < m_EdgeArray[i]->m_Node->m_Cost)
			{
				m_EdgeArray[i]->m_Node->m_Cost = m_Cost + m_EdgeArray[i]->m_Cost;
				m_EdgeArray[i]->m_Node->m_Parent = this;
			}

			// 방문을 위한 노드라면
			// heap에 넣어서, 연결된 노드를 판단해본다.
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
public :
	CDijstra()
	{
		m_Size = 0;
		m_Capacity = 4;
		m_NodeArray = new CDijkstraNode<T*>[m_Capacity];
	}
	~CDijkstra()
	{
		for(int i = 0 ; i < m_Size; i++)
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
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CDijkstraNode<T>** Array = new CDijkstraNode<T>*[m_Capacity];
			memcpy(Array, m_NodeArray, sizeof(CDijkstraNode<T>*) * m_Size);
			delete[] m_NodeArray;
			m_NodeArray = Array;
		}
	}
};

