#pragma once
#include <assert.h>

template<typename T>
class CStackNode {
	template<typename T>
	friend class CStack;
private :
	CStackNode(): Next(nullptr){}
	~CStackNode(){}
private :
	T Data;
	CStackNode<T>* Next;
};


template<typename T>
class CStack {
public:
	CStack()
	{
	}
	~CStack()
	{
		while (!empty())
		{
			pop();
		}
	}
private :
	CStackNode<T>* m_LastNode;
	int m_Size;
public :
	bool empty() const
{
		return m_Size == 0;
}
	void push(const T& Data)
{
		CStackNode<T>* NewNode = new CStackNode<T>;
		NewNode->Data = Data;

		if (!m_LastNode)
			m_LastNode = NewNode;
		else
		{
		NewNode->Next = m_LastNode;
		m_LastNode = NewNode;
		}
		++m_Size;
}
	T& top() const
{
		if (empty())
			assert(false);

		return m_LastNode->Data;
}
	void clear()
{
		while (m_LastNode)
		{
			CStackNode<T>* Next = m_LastNode->Next;
			delete m_LastNode;
			m_LastNode = Next;
		}
		m_Size = 0;
}
	void pop()
{
		if (empty())
			assert(false);

		CStackNode<T>* NextNode = m_LastNode->Next;
		delete m_LastNode;
		m_LastNode = NextNode;
		--m_Size;
}
};