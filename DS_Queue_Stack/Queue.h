#pragma once
#include<assert.h>

template<typename T>
class CQueueNode
{
public :
	CQueueNode()
	{
		m_Next = nullptr;
	}
	~CQueueNode()
	{

	}
private :
	T m_Data;
	CQueueNode<T>* m_Next;
};


template<typename T>
class CQueue
{
public :
	CQueue()
	{
		m_FirstNode = nullptr;
		m_LastNode = nullptr;
		m_Size = 0;
	}
	~CQueue()
	{
		clear();
	}
private :
	CQueueNode<T>* m_FirstNode;
	CQueueNode<T>* m_LastNode;
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear()
	{
		while (m_FirstNode)
		{
			CQueueNode<T>* Next = m_FirstNode->m_Next;
			delete m_FirstNode;
			m_FirstNode = Next;
		}
		m_LastNode = nullptr;
		m_Size = 0;
	}
	void push(const T& data)
	{
		CQueueNode<T>* Node = new CQueueNode;
		Node->m_Data = data;
		if (!m_FirstNode)
			m_FirstNode = Node;
		if (m_LastNode)
			m_LastNode->m_Next = Node;
		m_LastNode = Node;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		CQueueNode<T>* Next = m_FirstNode->m_Next;
		delete m_FirstNode;
		m_FirstNode = Next;
		if (!m_FirstNode) m_LastNode = nullptr;
		--m_Size;
	}
};