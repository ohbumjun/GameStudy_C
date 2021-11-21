#include<iostream>
#include<assert.h>

template<typename T, int SIZE = 200>
class CCircleQueue
{
public :
	CCircleQueue() :
		m_Capacity(SIZE + 1),
		m_Head(0),
		m_Tail(0),
		m_Size(0)
	{

	}
	~CCircleQueue()
	{
		delete[] m_Queue;
	}
private :
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;
	T m_Queue[SIZE + 1];
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	T& front() const
	{
		if (empty()) assert(false);
		int Head = (m_Head + 1) % m_Capacity;
		return m_Queue[Head];
	}
	void push(const T& data)
	{
		int Tail = (m_Tail + 1) % m_Capacity;
		if (Tail == m_Head) return;
		m_Queue[Tail] = data;
		m_Tail = Tail;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		m_Head = (m_Head + 1) % m_Capacity;
		++m_Size;
	}
	void clear()
	{
		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;
	}
};