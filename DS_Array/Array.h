#pragma once
#include<assert.h>

template<typename T>
class CArray
{
public :
	CArray(){
		m_Size = 0;
		m_Capacity = 4;
		m_Array = new T[m_Capacity];
	}
	CArray(const CArray& Array)
	{
		m_Size = Array.m_Size;
		m_Capacity = Array.m_Capacity;
		m_Array = new T[m_Capacity];
		memcpy(m_Array, Array.m_Array, sizeof(T) * m_Size);
	}
	~CArray(){
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void push_back(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];
			memcpy(Array, m_Array, sizeof(T) * m_Size);
			delete[] m_Array;
			m_Array = Array;
		}
		m_Array[m_Size] = data;
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		--m_Size;
	}
	void operator = (const CArray& Array)
	{
		m_Size = Array.m_Size;
		m_Capacity = Array.m_Capacity;
		delete[] m_Array;
		m_Array = new T[m_Capacity];
		memcpy(m_Array, Array.m_Array, sizeof(T) * m_Size);
	}
	void operator [] (int Index)
	{
		if (Index < 0 || Index >= m_Size) return;
		return m_Array[Index];
	}
	void clear()
	{
		for (int i = 0; i < m_Size; i++)
		{

		}
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
		if (Index == -1) return false;
		return erase(Index);
	}
	bool erase(const int Index)
	{
		if (Index < 0 || Index >= m_Size) return false;
		for (int i = Index; i < m_Size; i++)
		{
			m_Array[i] = m_Array[i + 1];
		}
		--m_Size;
	}
	void sort((*pFunc)(const T&, const T&))
	{
		for (int i = 0; i < m_Size - 1; i++)
		{
			for (int j = i + 1; j < m_Size; j++)
			{
				if (pFunc(m_Array[i], m_Array[j]))
				{
					T Temp = m_Array[i];
					m_Array[i] = m_Array[j];
					m_Array[j] = Temp;
				}
			}
		}
	}
};

