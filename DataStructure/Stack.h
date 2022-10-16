#pragma once
#include<assert.h>

template<typename T>
class CStackNode
{
	template<typename T>
	friend class CStack;
public :
	CStackNode() :
		m_NextNode(nullptr){}
	~CStackNode(){}
private :
	CStackNode<T>* m_NextNode;
	T m_Data;
};

template<typename T>
class CStack
{
	CStack() : 
		m_LastNode(nullptr),
		m_Size(0){}
	~CStack()
	{
		clear();
	}
private :
	CStackNode<T>* m_LastNode;
	int m_Size;
public :
	bool empty() const { return m_Size == 0; } //
	void push(const T& data)
	{
		CStackNode<T>* NewNode = new CStackNode<T>;
		NewNode->m_Data = data;
		NewNode->m_NextNode = m_LastNode;
		m_LastNode = NewNode;
		++m_Size;
	}
	T& top() const
	{
		if (empty()) assert(false);
		return m_LastNode->m_Data;
	}
	void pop()
	{
		if (empty()) assert(false);
		CStackNode<T>* NextNode = m_LastNode->m_NextNode; /////
		delete m_LastNode;
		m_LastNode = NextNode;
		--m_Size;
	}
	void clear()
	{
		while (m_LastNode)
		{
			CStackNode<T>* NextNode = m_LastNode->m_NextNode;
			delete NextNode;
			m_LastNode = NextNode;
		}
		m_Size = 0;
	}
};