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
private: 
	int m_Size;
	int m_Capacity;
	T* m_Array;
	bool (*m_Func)(const T&, const T&);
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void clear() { m_Size = 0; }
	T& top() const 
	{
		if (empty()) assert(false);
		return m_Array[0];
	}
	void pop()
	{
		if (empty()) assert(false);
		if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}
		--m_Size;
		m_Array[0] = m_Array[m_Size];
		DeleteHeap(0);
	}
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];
			memcpy(Array, m_Array, sizeof(T) * m_Capacity);
			delete [] m_Array;
			m_Array = Array;
		}
		m_Array[m_Size] = data;
		InsertHeap(m_Size);
		++m_Size;
	}

private :
	void InsertHeap(int Index)
	{
		if (Index == 0) return;
		int ParentIndex = (Index - 1) / 2;
		if (m_Func(m_Array[ParentIndex], m_Array[Index]))
		{
			T Temp = m_Array[Index];
			m_Array[Index] = m_Array[ParentIndex];
			m_Array[ParentIndex] = Temp;
			InsertHeap(ParentIndex);
		}
	}
	void DeleteHeap(int Index)
	{
		int LeftChildIdx = Index * 2 + 1;
		if (LeftChildIdx >= m_Size) return;
		int RightChildIdx = LeftChildIdx + 1;
		int ChildIdx = LeftChildIdx;
		if (RightChildIdx < m_Size)
		{
			if (m_Func(m_Array[LeftChildIdx], m_Array[RightChildIdx]))
				ChildIdx = RightChildIdx;
		}
		if (m_Func(m_Array[Index], m_Array[ChildIdx]))
		{
			T Temp = m_Array[Index];
			m_Array[Index] = m_Array[ChildIdx];
			m_Array[ChildIdx] = Temp;
			InsertHeap(ChildIdx);
		}
	}
public :
	void SetSortFunction(bool (*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};