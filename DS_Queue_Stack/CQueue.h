#pragma once
#include<assert.h>


template<typename T, int SIZE = 200>
class CQueue
{
private :
	typedef CQueueNode<T> NODE;
	typedef CQueueNode<T>* PNODE;
public :
	CQueue()
	{
		m_Size = 0;
		m_Capacity = SIZE + 1;
		m_Head = 0;
		m_Tail = 0;
		m_Array = new T[m_Capacity];
	}
	~CQueue()
	{
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	int m_Head;
	int m_Tail;
	T* m_Array[SIZE + 1];
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear()
	{
		if (empty()) assert(false);
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
	void clear()
	{
		m_Size = 0;
		m_Head = 0;
		m_Tail = 0;
	}
};