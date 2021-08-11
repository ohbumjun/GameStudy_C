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

	}
private :
	int m_Size;
	CQueueNode<T>* m_FirstNode;
	CQueueNode<T>* m_LastNode;
public :
	int size() const { return m_Size; }
};