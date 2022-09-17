#pragma once
#include<assert.h>
#include"Stack.h"
#include"Queue.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraphNode;
	template<typename T>
	friend class CGraph;
public :
	CEdge()
	{
		m_Node = nullptr; //
	}
	~CEdge()
	{

	}
private :
	class CGraphNode<T>* m_Node;
};

template<typename T>
class CGraphNode
{
	template<typename T>
	friend class CGraph;
public :
	CGraphNode()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
		m_Visit = false;
	}
	~CGraphNode()
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
public :
	void AddEdge(CGraphNode<T>* Node)
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
		m_EdgeArray[m_Size] = Edge;
		++m_Size;
	}
	void Add(CStack<CGraphNode<T>*>& stack)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			stack.push(m_EdgeArray[i]->m_Node);
			m_EdgeArray[i]->m_Node->m_Visit = true;
		}
	}
	void Add(CQueue<CGraphNode<T>*>& queue)
	{
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			queue.push(m_EdgeArray[i]->m_Node);
			m_EdgeArray[i]->m_Node->m_Visit = true;
		}
	}
};

template<typename T>
class CGraph
{
public :
	CGraph()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_NodeArray = new CGraphNode<T>*[m_Capacity];
	}
	~CGraph()
	{
		for (int i = 0; i < m_Size; i++)
			delete m_NodeArray[i];
		delete[] m_NodeArray;
	}
private:
	int m_Size;
	int m_Capacity;
	CGraphNode<T>** m_NodeArray;
public :
	void insert(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CGraphNode<T>** Array = new CGraphNode<T>*[m_Capacity];
			memcpy(Array, m_NodeArray, sizeof(CGraphNode<T>*) * m_Size);
			delete[] m_NodeArray;
			m_NodeArray = Array;
		}
		CGraphNode<T>* Node = new CGraphNode<T>;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
		++m_Size;
	}
	void AddEdge(const T& Src, const T& Dest)
	{
		CGraphNode<T>* SrcNode  = nullptr;
		CGraphNode<T>* DestNode = nullptr;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
				SrcNode = m_NodeArray[i];
			else if (m_NodeArray[i]->m_Data == Dest)
				DestNode = m_NodeArray[i];
			if (SrcNode && DestNode) break;
		}
		if (!SrcNode || !DestNode) return;
		SrcNode->AddEdge(DestNode);
		DestNode->AddEdge(SrcNode);
	}
	void BFS(void (*pFunc)(const T&)) ///////
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
			m_NodeArray[i]->m_Visit = false;
		CQueue<CGraphNode<T>*> queue;
		queue.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!queue.empty())
		{
			CGraphNode<T>* Node = queue.front();
			queue.pop();
			Node->Add(queue);
			pFunc(Node->m_Data);
		}
	}

	void DFS(void (*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
			m_NodeArray[i]->m_Visit = false; ////
		CStack<CGraphNode<T>*> stack;
		stack.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!stack.empty()) ////
		{
			CGraphNode<T>* Node = stack.top();
			stack.pop();
			Node->Add(stack);
			pFunc(Node->m_Data);
		}
	}
};