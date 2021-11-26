#pragma once

#include<assert.h>

template<typename T>
class CQueueNode
{
	template<typename T>
	friend class CQueue;
public :
	CQueueNode() : 
		m_NextNode(nullptr)
	{
	}
private :
	T m_Data;
	CQueueNode<T>* m_NextNode;
};

template<typename T>
class CQueue
{
public :
	CQueue() :
		m_LastNode(nullptr),
		m_FirstNode(nullptr){}
	~CQueue()
	{
		CQueueNode<T>* DeleteNode = m_FirstNode;
		while (DeleteNode)
		{
			CQueueNode<T>* NextNode = DeleteNode->m_NextNode;
			delete NextNode;
			DeleteNode = NextNode;
		}
	}
private :
	CQueueNode<T>* m_FirstNode;
	CQueueNode<T>* m_LastNode;
public :
	void push(const T& data)
	{
		CQueueNode<T>* NewNode = new CQueueNode<T>;
		NewNode->m_Data = data;

		NewNode->m_NextNode = m_LastNode;
		if (!m_FirstNode)
			m_FirstNode = NewNode;

		m_LastNode = NewNode;
		++m_Size;
	}
	bool empty() const { return m_Size == 0; }
	T& top()
	{
		if (empty()) assert(false);
		return m_FirstNode->m_Data;
	}
	void pop()
	{
		if (empty()) assert(fase);
	}
};