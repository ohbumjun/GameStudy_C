#pragma once

#include<assert.h>

template<typename T>
class CQueueNode
{
	template<typename T>
	friend class CQueue;
public :
	CQueueNode() 
	{
		m_Next = nullptr;
		m_Prev = nullptr;
	}
	~CQueueNode()
	{

	}
private :
	CQueueNode<T>* m_Next;
	CQueueNode<T>* m_Prev;
	T m_Data;
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
		while (m_FirstNode)
		{
			CQueueNode<T>* Next = m_FirstNode->m_Next;
			delete m_FirstNode;
			m_FirstNode = Next;
		}
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
	T& front() const
	{
		if (empty()) assert(false);
		return m_FirstNode->m_Data;
	}
	void push(const T& data)
	{
		CQueueNode<T>* Node = new CQueueNode<T>;
		Node->m_Data = data;

		if (!m_FirstNode) m_FirstNode = Node;
		if (m_LastNode) m_LastNode->m_Next = Node;
		m_LastNode = Node;
		++m_Size;
	}
	void pop()
	{
		CQueueNode<T>* Next = m_FirstNode->m_Next;
		delete m_FirstNode;
		m_FirstNode = Next;
		if (!m_FirstNode) m_LastNode = nullptr;
		--m_Size;
	}
};