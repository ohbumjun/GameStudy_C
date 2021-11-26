#pragma once

#include<assert.h>
#include "Queue.h"
#include "Stack.h"

template<typename T>
class CEdge
{
	template<typename T>
	friend class CGraph;
	template<typename T>
	friend class CGraphNode;
public :
	CEdge() : m_Node(nullptr){}
	~CEdge(){}
private :
	CGraphNode<T>* m_Node;
};

template<typename T>
class CGraphNode
{
	template<typename T>
	friend class CGraph;
public :
	CGraphNode()
	{
		m_Size       = 0;
		m_Capacity = 4;
		m_Visit       = false;
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
	CEdge<T>** m_EdgeArray;
	int m_Size;
	int m_Capacity;
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
		CEdge<T>* NewEdge = new CEdge<T>;
		NewEdge->m_Node = Node;
		m_EdgeArray[m_Size] = Edge;
		++m_Size;
	}
	void Add(CQueue<T>* queue)
	{
		// 规巩 贸府 促 false
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit)
				continue;
			m_EdgeArray[i]->m_Node->m_Visit = true;
			queue->push(m_EdgeArray[i]->m_Node);
		}
	}
	void Add(CStack<T>* stack)
	{
		// 规巩 贸府 促 false
		for (int i = 0; i < m_Size; i++)
		{
			if (m_EdgeArray[i]->m_Node->m_Visit)
				continue;
			m_EdgeArray[i]->m_Node->m_Visit = true;
			stack->push(m_EdgeArray[i]->m_Node);
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
			delete m_NodeArray[i];
		delete[] m_NodeArray;
	}
private:
	CGraphNode<T>** m_NodeArray;
	int m_Size;
	int m_Capacity;
public :
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			CGraphNode<T>** NewArray = new CGraphNode<T>*[m_Capacity];
			memcpy(NewArray, m_NodeArray, sizeof(CGraphNode<T>));
			delete[] m_NodeArray;
			m_NodeArray = NewArray;
		}
		
		CGraphNode<T>* Node = new CGraphNode<T>;
		Node->m_Data = data;
		m_NodeArray[m_Size] = Node;
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
	void BFS(void (*pFunc)(const T&))
	{
		if (m_Size == 0) return;
		for (int i = 0; i < m_Size; i++)
			m_NodeArray[i]->m_Visit = false;

		CQueue<T> queue;
		queue.push(m_NodeArray[0]);
		m_NodeArray[0]->m_Visit = true;
		while (!queue)
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
			m_NodeArray[i]->m_Visit = false;

		CStack<T> stack;
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
};