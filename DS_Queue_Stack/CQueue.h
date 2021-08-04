#pragma once
#include<assert.h>

template<typename T, int SIZE = 100>
class CCircleQueue
{
public :
	CCircleQueue(){
		m_Size = 0;
		m_Capacity = SIZE + 1;
		m_Queue = new T[m_Capacity];
		m_Head = 0;
		m_Tail = 0;
	}
	~CCircleQueue() { clear(); }
private :
	int m_Size;
	int m_Capacity;
	T* m_Queue[SIZE + 1];
	int m_Head;
	int m_Tail;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0;}
	void push(const T& data)
	{
		int Tail = (m_Tail + 1) % m_Capacity;
		if (Tail == m_Head) return;
		m_Queue[Tail] = data;
		m_Tail = Tail;
		++m_Size;
	}
	T& front() const 
	{
		if (empty()) assert(false);
		int Head = (m_Head + 1) % m_Capacity;
		return m_Queue[Head];
	}
	void pop()
	{
		if (empty()) assert(false);
		m_Head = (m_Head + 1) % m_Capacity;
		--m_Size;
	}
	void clear()
	{
		m_Size = m_Head = m_Tail = 0;
	}
};