#pragma once
#include<assert.h>

template<typename T>
class CQuickSort
{
public :
	CQuickSort()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Data = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CQuickSort()
	{
		delete[] m_Data;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Data;
	bool (*m_Func)(const T&, const T&);
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
public :
	void SetSortFunction(bool(*pFunc)(const T& , const T&))
	{
		m_Func = pFunc;
	}
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
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
		++m_Size;
	}
	void push(T* Array, int Count)
	{
		if (m_Size <= Count)
		{
			m_Size = Count;
			delete[] m_Data;
			m_Data = new T[Count];
		}
		for (int i = 0; i < Count; i++)
		{
			m_Data[i] = Array[i];
		}
	}
	void Sort()
	{
		QuickSort(0, m_Size - 1, m_Data);
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
			QuickSort(Pivot + 1, Right, Array);
			QuickSort(Left, Pivot - 1, Array);
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
			} while (Low <= Right && m_Func(Value,Array[Low]));
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
};