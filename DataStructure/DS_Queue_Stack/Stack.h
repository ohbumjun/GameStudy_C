#pragma once
#include<assert.h>

template<typename T>
class CStackNode
{
	template<typename T>
	friend class CStack;
public :
	CStackNode()
	{
		m_Next = nullptr;
	}
	~CStackNode()
	{

	}
private :
	CStackNode<T>* m_Next;
	T m_Data;
};

template<typename T>
class CStack
{
public :
	CStack()
	{
		m_LastNode = nullptr;//
		m_Size = 0;
	}
	~CStack()
	{
	}
private :
	int m_Size;
	CStackNode<T>* m_LastNode;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	T& top() const
	{
		if (empty()) assert(false);
		return m_LastNode->m_Data;
	}
	void pop()
	{
		if (empty()) assert(false);
		CStackNode<T>* Next = m_LastNode->m_Next;
		delete m_LastNode;
		m_LastNode = Next;
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
	void push(const T& data)
	{
		CStackNode<T>* NewNode = new CStackNode<T>;
		NewNode->m_Data = data;
		NewNode->m_Next = m_LastNode;
		m_LastNode = NewNode;
		++m_Size;
	}
};