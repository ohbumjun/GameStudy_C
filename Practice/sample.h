#pragma once
#include<assert.h>

template<typename T>
class CArray
{
public :
	CArray()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Array = new T[m_Capacity];
	}
	CArray(const CArray& Array)
	{
		m_Size     = Array.m_Size;
		m_Capacity = Array.m_Capacity;
		m_Array = new T[m_Capacity];
		memcpy(m_Array, Array.m_Array, sizeof(T) * m_Size);
	}
	~CArray()
	{
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
public :	
	int size() const { return m_Size; }
	bool empty() const { return  m_Size == 0; }
	void clear()
	{
		m_Size = 0;
	}
	bool erase(const T& data)
	{
		int Index = -1;
		for (int i = 0; i < m_Size; i++)
		{
			if (m_Array[i] == data)
			{
				Index = i;
				break;
			}
		}
		if (Index == m_Size) return -1;
		return eraseIndex(Index);
	}
	bool eraseIndex(int Index)
	{
		if (Index < 0 || Index >= m_Size) return false;
		for (int i = Index; i < m_Size; i++)
		{
			m_Array[i] = m_Array[i + 1];
		}
		--m_Size;
		return true;
	}
	void operator = (const CArray<T>& Array)
	{
		delete[] m_Array;
		m_Size = Array.m_Size;
		m_Capacity = Array.m_Capacity;
		m_Array = new T[m_Capacity];
		memcpy(m_Array, Array, sizeof(T) * m_Size);
	}
};