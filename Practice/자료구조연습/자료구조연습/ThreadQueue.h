#pragma once
#include "CSync.h"
#include <assert.h>
#include <process.h>
#include <windows.h>

template<typename T, int SIZE = 200>
class CThreadQueue {
public :
	CThreadQueue()
{
		m_Size = 0;
		m_Capacity = SIZE + 1;
		m_Head = 0;
		m_Tail = 0;
		InitializeCriticalSection(&m_Crt);
}
	~CThreadQueue()
{
		DeleteCriticalSection(&m_Crt);
}
private :
	int m_Size;
	int m_Capacity;
	T m_Data[SIZE + 1];
	int m_Head;
	int m_Tail;
	CRITICAL_SECTION m_Crt;
public :
	void clear()
{
		CSync Sync(&m_Crt);
		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;
}
	bool empty() const
{
		CSync Sync(&m_Crt);
		return m_Size == 0;
}
	void push(const T& Data)
{
		CSync Sync(&m_Crt);
		int Tail = (m_Tail + 1) % m_Capacity;

		if (Tail == m_Head)
			return;

		m_Data[Tail] = Data;
		m_Tail = Tail;
		++m_Size;
}
	void pop()
{
	CSync Sync(&m_Crt);
	if (empty())
	{
		assert(false);
	}
	m_Head = (m_Head + 1) % m_Capacity;
	--m_Size;
}
	T& front() const
{
	CSync Sync(&m_Crt);
	if (empty()) 
	{
		assert(false);
	}
	return m_Data[(m_Head + 1) % m_Capacity];
}
};