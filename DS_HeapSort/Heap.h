#pragma once
#include<assert.h>

template<typename T>
class CHeapSort
{
public :
	CHeapSort()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Array = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CHeapSort()
	{
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	bool (*m_Func)(const T&, const T&);
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear() { m_Size = 0; }
	void push(const T& data)
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
		InsertHeap(m_Size);
		++m_Size;
	}
	void pop()
	{
		if (empty()) assert(false);
		else if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}
		--m_Size;
		m_Array[0] = m_Array[m_Size];
		DeleteHeap(0);
	}
private :
	void InsertHeap(int Index)
	{
		if (Index == 0) return;
		int ParentIndex = (Index - 1) / 2;
		if (m_Func(m_Array[ParentIndex], m_Array[Index]))
		{
			T temp = m_Array[ParentIndex];
			m_Array[ParentIndex] = m_Array[Index];
			m_Array[Index] = temp;
			InsertHeap(ParentIndex);
		}
	}
	void DeleteHeap(int Index)
	{
		int LeftChildIndex = (Index * 2) + 1;
		if (LeftChildIndex >= m_Size)
			return;
		int RightChildIndex = LeftChildIndex + 1;
		int ChildIndex = LeftChildIndex;
		if (RightChildIndex < m_Size)
		{
			if (m_Func(m_Array[RightChildIndex], m_Array[LeftChildIndex]))
				ChildIndex = RightChildIndex;
		}
		if (m_Func(m_Array[ChildIndex], m_Array[Index]))
		{
			T temp = m_Array[ChildIndex];
			m_Array[ChildIndex] = m_Array[Index];
			m_Array[Index] = temp;
			DeleteHeap(ChildIndex);
		}
	}
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
public :
	void SetSortFunction(bool(*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}
};