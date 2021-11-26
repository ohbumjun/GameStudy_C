#pragma once

#include "Stack.h"
#include "Queue.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraphNode;
	template<typename T>
	friend class CGraph;
public :
	CEdge() : m_Node(nullptr)
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
		m_Visit = false;
		m_Size = 0;
		m_Capacity = 4;
		m_EdgeArray = new CEdge<T>*[m_Capacity];
	}
	~CGraphNode()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_EdgeArray[i];
		}
		delete[] m_EdgeArray;
	}
private :
	T m_Data;
	bool m_Visit;
	int m_Size;
	int m_Capacity;
	CEdge<T>** m_EdgeArray;
public :
	bool empty() const { return m_Size == 0; }
	void AddEdge(const CGraphNode<T>* Node)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CEdge<T>** NewArray = new CEdge<T>*[m_Capacity];
			memcpy(NewArray, m_EdgeArray, sizeof(CEdge<T>*) * m_Size);
			delete[] m_EdgeArray;
			m_EdgeArray = Array;
		}
		CEdge<T>* NewEdge = new CEdge<T>;
		NewEdge->m_Node = Node;
		m_EdgeArray[m_Size] = NewEdge;
		++m_Size;
	}
	void Add(CStack<T>& stack)
	{
		// 나의 연결된 애들은 stack에 넣어야 한다.
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			m_EdgeArray[i]->m_Node->m_Visit = true;
			stack.push(m_EdgeArray[i]->m_Node);
		}
	}
	void Add(CQueue<T>& queue)
	{
		// 나의 연결된 애들은 stack에 넣어야 한다.
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit) continue;
			m_EdgeArray[i]->m_Node->m_Visit = true;
			queue.push(m_EdgeArray[i]->m_Node);
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
		m_Capacity = 4;
		m_NodeArray = new CGraphNode<T>*[m_Capacity];
	}
	~CGraph()
	{
		for (int i = 0; i < m_Size; i++)
		{
			delete m_NodeArray[i];
		}
		delete [] m_NodeArray;
	}
private :
	int m_Size;
	int m_Capacity;
	CGraphNode<T>** m_NodeArray;
public :
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CGraphNode<T>** NewArray = new CGraphNode<T>*[m_Capacity];
			memcpy(NewArray, m_NodeArray, sizeof(CGraphNode<T>*) * m_Size);
			delete[] m_NodeArray;
			m_NodeArray = NewArray;
		}
		CGraphNode<T>* NewNode = new CGraphNode<T>;
		NewNode->m_Data = data;
		m_NodeArray[m_Size] = NewNode;
		++m_Size;
	}
	void AddEdge(const T& Src, const T& End)
	{
		CGraphNode<T>* SrcNode = nullptr;
		CGraphNode<T>* EndNode = nullptr;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_NodeArray[i]->m_Data == Src)
				SrcNode = m_NodeArray[i];
			else if (m_NodeArray[i]->m_Data == End)
				EndNode = m_NodeArray[i];
			if (SrcNode && EndNode) break;
		}
		if (!SrcNode || !EndNode) return;
		SrcNode->AddEdge(EndNode);
		EndNode->AddEdge(SrcNode);
	}
	void DFS(void (*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
			m_NodeArray[i]->m_Visit = false;

		CStack<CGraphNode<T>*> stack;
		stack.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!stack)
		{
			CGraphNode<T>* Node = stack.top();
			stack.pop();
			Node->Add(stack);
			pFunc(Node->m_Data);
		}
	}
	void BFS(void (*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
			m_NodeArray[i]->m_Visit = false;

		CQueue<CGraphNode<T>*> queue;
		queue.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!stack)
		{
			CGraphNode<T>* Node = queue.top();
			queue.pop();
			Node->Add(queue);
			pFunc(Node->m_Data);
		}
	}
};