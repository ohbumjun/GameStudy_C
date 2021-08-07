#pragma once
#include<assert.h>

template<typename T>
class CQuick
{
public :
	CQuick()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Data = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CQuick()
	{
		delete[] m_Data;
	}
private :
	T* m_Data;
	int m_Size;
	int m_Capacity;
	bool (*m_Func)(const T&, const T&);
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
public :
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void push(const T& data)
	{
		if (m_Capacity == m_Size)
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
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[] m_Data;
			m_Data = new T[m_Capacity];
		}
		for (int i = 0; i < m_Size; i++)
		{
			m_Data[i] = Array[i];
		}
		m_Size = Count;
	}
	void Sort()
	{
		QuickSort(0, m_Size - 1; m_Data);
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
			
			// 피봇기준으로 
			// 왼쪽,오른쪽 분할해서 들어간다
			QuickSort(Left,Pivot-1,Array);
			QuickSort(Pivot + 1, Right, Array);
		}
	}
	int Partition(int Left, int Right, T* Array)
	{
		int Low = Left;
		int High = Right + 1;
		T Value = Array[Low];

		do
		{
			do
			{
				++Low;
			} while (Low <= Right && m_Func(m_Data[Value],m_Data[Low]));
			do
			{
				--High;
			} while (High >= Low && m_Func(m_Data[High],m_Data[Value]));
			if (Low < High)
			{
				T temp = Array[Low];
				Array[Low] = Array[High];
				Array[High] = temp;
			}
		} while (Left < High);
	}
};