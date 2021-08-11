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
		m_Size = 0;
		m_FirstNode = nullptr;
		m_LastNode = nullptr;
	}
	~CQueue()
	{
		clear();
	}
private :
	int m_Size;
	CQueueNode<T>* m_FirstNode;
	CQueueNode<T>* m_LastNode;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear()
	{
		while (m_FirstNode)
		{
			CQueueNode<T> Next = m_FirstNode->m_Next;
			delete m_FirstNode;
			m_FirstNode = Next;
		}
		m_Size = 0;
		m_LastNode = nullptr;
	}
	void push(const T& data)
	{
		CQueueNode<T> Node = new CQueueNode<T>;
		Node->m_Data = data;
		if (!m_FirstNode) m_FirstNode = Node;
		if (m_LastNode) Node->m_Next = m_LastNode;
		m_LastNode = Node;
		++m_Size;
	}
	T& top() const
	{
		if (empty()) return;
		return m_FirstNode->m_Data;
	}
	void pop()
	{
		if (empty()) return;
		delete 
	}
};