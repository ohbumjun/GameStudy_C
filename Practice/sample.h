#pragma once
#include<assert.h>

template<typename T, int SIZE = 10>
class CCircleQueue
{
public :
	CCircleQueue()
	{
		m_Size = 0;
		m_Capacity = SIZE+1;
		m_Array = new T[m_Capacity];
		m_Tail = 0;
		m_Head = 0;
	}
	~CCircleQueue()
	{
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	int m_Tail;
	int m_Head;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear() { m_Size = 0; }
	T& front() const
	{
		if (empty()) assert(false);
		int Head = (m_Head + 1) % m_Capacity;
		return m_Array[Head];
	}
	void push(const T& data)
	{
		int Tail = (m_Tail + 1) % m_Capacity;
		if (Tail == m_Head) return;
		m_Array[Tail] = data;
		m_Tail = Tail;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		m_Head = (m_Head + 1) % m_Capacity;
		--m_Size;
	}
};