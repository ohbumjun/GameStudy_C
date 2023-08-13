#pragma once

#include <assert.h>

template<typename T>
class CQueueNode {
	template <typename T>
	friend class CQueue;
private :
	T Data;
	CQueueNode<T>* m_Next;
};

template<typename T>
class CQueue {
	typedef CQueueNode<T> NODE;
	typedef CQueueNode<T>* PNODE;
private :
	PNODE m_FirstNode;
	PNODE m_LastNode;
	int m_Size;
public :
	void push(const T& data)
{
		PNODE NewNode = new NODE;
		NewNode->Data = data;

		if (!m_FirstNode)
			m_FirstNode = NewNode;
		if (m_LastNode)
			m_LastNode->m_Next = NewNode;
		m_LastNode = NewNode;
		++m_Size;
}
	void pop()
{
		if (empty())
			assert(false);

		PNODE Next = m_FirstNode->m_Next;
		delete m_FirstNode;
		m_FirstNode = Next;

		if (!m_FirstNode)
			m_LastNode = nullptr;

		--m_Size;
}
	T& front() const
{
		if (empty())
			assert(false);

		return m_FirstNode->Data;
}
	bool empty() const
{
		return m_Size == 0;
}
	void clear()
{
	while (m_FirstNode)
	{
		PNODE NextNode = m_FirstNode->m_Next;
		delete m_FirstNode;
		m_FirstNode = NextNode;
	}

	m_LastNode = nullptr;
	m_Size = 0;
}
};