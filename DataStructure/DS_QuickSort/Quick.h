#pragma once
#include<assert.h>
#include<iostream>

template<typename T>
class CQuickSort
{
public:
	CQuickSort()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Array = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CQuickSort()
	{
		delete[] m_Array;
	}
private:
	int m_Size;
	int m_Capacity;
	T* m_Array;
	bool (*m_Func)(const T&, const T&);
private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
public:
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
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
		++m_Size;
	}
	void push(const T* Array, int Count)
	{
		if (Count > m_Capacity)
		{
			m_Capacity = Count;
			delete[] m_Array;
			m_Array = new T[m_Capacity];

		}
		for (int i = 0; i < Count; i++)
		{
			m_Array[i] = Array[i];
		}
		m_Size = Count;
	}
	void pop()
	{
		if (empty()) assert(false);
		--m_Size;
	}
	void Sort()
	{
		QuickSort(0, m_Size - 1, m_Array);
	}
	void Sort(T* Array, int Count)
	{
		QuickSort(0, Count - 1, Array);
	}
	void QuickSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			int Pivot = Partition(Left, Right, Array);
			QuickSort(Left, Pivot - 1, Array);
			QuickSort(Pivot + 1, Right, Array);
		}
	}
	int Partition(int Left, int Right, T* Array)
	{
		int Low = Left;
		int High = Right + 1;
		T Value = Array[Left];
		do
		{
			do
			{
				++Low;
			} while (Low <= Right && m_Func(Value, Array[Low]));
			do
			{
				--High;
			} while (High >= Left && m_Func(Array[High], Value));
			if (Low < High)
			{
				T temp = Array[Low];
				Array[Low] = Array[High];
				Array[High] = temp;
			}
		} while (Low < High);

		T temp = Array[High];
		Array[High] = Array[Left];
		Array[Left] = temp;
		return High;
	}
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
public :
	
	void SetSortFunction(bool (*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}
};
