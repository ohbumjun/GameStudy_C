#pragma once
#include<assert.h>

template<typename T>
class CStackNode
{
public :
	CStackNode()
	{
		m_Next = nullptr;
	}
	~CStackNode()
	{

	}
private :
	T m_Data;
	CStackNode<T>* m_Next;
};

template<typename T>
class CStack
{
public :
	CStack()
	{
		m_LastNode = nullptr;
		m_Size = 0;
	}
	~CStack()
	{
		clear();
	}
private :
	CStackNode<T>* m_LastNode;
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void push(const T& data)
	{
		CStackNode<T>* Node = new CStackNode;
		Node->m_Data = data;
		Node->m_Next = m_LastNode;
		m_LastNode = Node;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		CStackNode<T>* Next = m_LastNode->m_Next;
		delete m_LastNode;
		m_LastNode = Next;
		--m_Size;
	}
	void clear()
	{
		while (m_LastNode)
		{
			CStackNode<T>* Next = m_LastNode->m_Next;
			delete m_LastNode;
			m_LastNode = Next;
		}
		m_Size = 0;
	}
};