#pragma once
#include<assert.h>

template<typename T>
class CHeapSort
{
public:
	CHeapSort()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Data = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CHeapSort()
	{
		delete[] m_Data;
	}
private : 
	void SetSortFunction(bool(*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}
private:
	int m_Size;
	int m_Capacity;
	T* m_Data;
	bool (*m_Func)(const T&, const T&);
public:
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear() { m_Size = 0; }
	T top() const 
	{
		if (empty()) assert(false);
		return m_Data[0];
	}
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];
			memcpy(Array, m_Data, sizeof(T) * m_Size);
			delete[] m_Data;
			m_Data = Array;
		}
		m_Data[m_Size] = data;
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
		m_Data[0] = m_Data[m_Size];
		DeleteHeap(0);
	}
	void InsertHeap(int Index)
	{
		if (Index == 0) return;
		int ParentIndex = (Index - 1) / 2;
		if (m_Func(m_Data[Index], m_Data[ParentIndex]))
		{
			T temp = m_Data[Index];
			m_Data[Index] = m_Data[ParentIndex];
			m_Data[ParentIndex] = temp;
			InsertHeap(ParentIndex);
		}
	}
	void DeleteHeap(int Index)
	{
		int LeftChildIndex = (Index * 2) + 1;
		if (LeftChildIndex >= m_Size) return;

		int RightChildIndex = LeftChildIndex + 1;
		int ChildIndex = LeftChildIndex;
		if (RightChildIndex < m_Size)
		{
			if (m_Func(m_Data[RightChildIndex], m_Data[LeftChildIndex]))
				ChildIndex = RightChildIndex;
		}
		if (m_Func(m_Data[ChildIndex], m_Data[Index]))
		{
			T temp = m_Data[ChildIndex];
			m_Data[ChildIndex] = m_Data[Index];
			m_Data[Index] = temp;
			DeleteHeap(ChildIndex);
		}
	}; //

private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	};
};