#pragma once 
#include"Heap.h"
#include"Quick.h"

template<typename T>
class CMergeSort
{
public :
	CMergeSort()
	{
	}
	~CMergeSort()
	{

	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	T* m_CopyArray;
	bool (*m_Func)(const T&, const T&);
public :
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
			delete[] m_Capacity;
			m_Array = Array;
			m_CopyArray = new T[m_Capacity];
		}
		m_CopyArray[m_Size] = data;
		++m_Size;
	}
	void push(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[] m_Array;
			delete[] m_CopyArray;
			m_Array = new T[m_Capacity];
			m_CopyArray = new T[m_Capacity];
		}
		for (int i = 0; i < Count; i++)
		{
			m_Array[i] = Array[i];
		}
		m_Size = Count;
	}
	void Sort()
	{
		MergeSort(0, m_Size - 1, m_Array);
	}
	void Sort(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[] m_Array;
			delete[] m_CopyArray;
			m_Array = new T[m_Capacity];
			m_CopyArray = new T[m_Capacity];
		}
		MergeSort(0, Count - 1, Array);
	}
private :
	void MergeSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			int Mid = (Left + Right) / 2;
			MergeSort(Left, Mid , Array);
			MergeSort(Mid + 1, Right, Array);
			Merge(Left, Mid, Right, Array);
		}
	}
	void Merge(int Left, int Mid, int Right, T* Array)
	{
		int Low  = Left;
		int High = Mid+1;
		int Pivot = Left;
		while (Low <= Mid && High <= Right)
		{
			if (m_Func(Array[Low], Array[High]))
			{
				m_CopyArray[Pivot] = Array[High];
				++High;
				++Pivot;
			}
			else
			{
				m_CopyArray[Pivot] = Array[Low];
				++Low;
				++Pivot;
			}
		}
		if (Low <= Mid)
		{
			for (int i = Low; i <= Mid; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}
		else if(High <= Right)
		{
			for (int i = High; i <= Right; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}
		for (int i = Left; i <= Right; i++)
		{
			m_Array[i] = m_CopyArray[i];
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