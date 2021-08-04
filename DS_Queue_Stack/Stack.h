#pragma once 
#include<assert.h>

template<typename T>
class CStackNode
{
	template<typename T>
	friend class CStack;
public :
	CStackNode():m_Next(nullptr){}
	~CStackNode(){}
private :
	CStackNode<T>* m_Next;
	T m_Data;
};

template<typename T>
class CStack
{
private :
	typedef CStackNode<T> NODE;
	typedef CStackNode<T>* PNODE;

public : 
	CStack() :m_LastNode(nullptr),m_Size(0) {}
	~CStack() { clear(); }
private :
	CStack<T>* m_LastNode;
	int m_Size;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void push(const T& data)
	{
		PNODE NewNode = new NODE;
		NewNode->m_Data = data;
		NewNode->m_Next = m_LastNode;
		m_LastNode = NewNode;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		PNODE Next = m_LastNode->m_Next;
		delete m_LastNode;
		m_LastNode = Next;
		--m_Size;
	}
	T& top() const
	{
		if (empty()) assert(false);
		return m_LastNode->m_Data;
	}
	void clear()
	{
		while (m_LastNode)
		{
			PNODE Next = m_LastNode->m_Next;
			delete m_LastNode;
			m_LastNode = Next;
		}
		m_Size = 0;
	}
};
