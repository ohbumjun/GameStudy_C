#pragma once

#include <assert.h>

template<typename T, int HASHSIZE = 200>
class CCircleQueue {
public :
	CCircleQueue()
{
		m_Capacity = HASHSIZE + 1;
		m_Size = 0;
		m_Tail = 0; m_Head = 0;
}
	~CCircleQueue()
{
	
}
private :
	T m_Array[HASHSIZE];
	int m_Capacity;
	int m_Size;
	int m_Tail;
	int m_Head;
public :
	int size() const
{
		return m_Size;
}
	bool empty() const
{
		return m_Size == 0;
}
	int capacity() const
{
		return m_Capacity;
}
	void push(const T& Data)
{
		int Tail = (m_Tail + 1) % m_Capacity;

		if (Tail == m_Head)
			return;

		m_Array[Tail] = Data;
		m_Tail = Tail;

		++m_Size;
}
	T& front() const
{
		if (empty())
			assert(false);

		return m_Array[(m_Head + 1) % m_Capacity];
}
	void pop()
{
		if (empty())
			assert(false); //

		--m_Size;
		++m_Head; //
}
};