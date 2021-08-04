#pragma once
#include<assert.h>

template<typename T>
class CQueueNode
{
	template<typename T>
	friend class CQueue;
public :
	CQueueNode() :m_Next(nullptr) {}
	~CQueueNode(){}
private :
	CQueueNode<T>* m_Next;
	T m_Data;
};

template<typename T>
class CQueue
{
private :
	typedef CQueueNode<T> NODE;
	typedef CQueueNode<T>* PNODE;
public :
	CQueue(){
		m_Size = 0;
		m_FirstNode = nullptr;
		m_LastNode = nullptr;
	}
	~CQueue() { clear(); }
private :
	int m_Size;
	PNODE m_FirstNode;
	PNODE m_LastNode;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void push(const T& data)
	{
		PNODE Node = new NODE;
		Node->m_Data = data;
		if (m_LastNode)
			m_FirstNode->m_Next = Node;
		if (!m_FirstNode) m_FirstNode = Node;
		m_LastNode = Node;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		PNODE Next = m_FirstNode->m_Next;
		delete m_FirstNode;
		m_FirstNode = Next;
		if (!m_FirstNode) m_LastNode = nullptr;
		--m_Size;
	}
	T& front() const
	{
		if (empty()) assert(false);
		return m_FirstNode->m_Data;
	}
	void clear()
	{
		while (m_FirstNode)
		{
			PNODE Next = m_FirstNode->m_Next;
			delete m_FirstNode;
			m_FirstNode = Next;
		}
		m_LastNode = nullptr;
		m_Size = 0;
	}
};