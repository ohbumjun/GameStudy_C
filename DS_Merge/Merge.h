#pragma once


/*
병합정렬의 경우 ,
배열 하나를 추가적으로
만들어야 한다.

병합정렬은 반씩 쪼개는 원리라고
생각하면 된다.

3 7 1 5 6 8 2 4

3 7 1 5			6 8 2 4

3 7		1 5		6 8		2 4

3	7	1	5	6	8	2	4

자. 오름차순 기준.
다시 오름차순 기준으로 합친다

37	15	68	24

37 ~ 15를 비교한다
1 3 5 7	    2 4 6 8 

1 2 3 4 5 6 7 8

즉, 우선 1개 단위로 까지 나누고,
되돌아오면서, 정렬을 적용하는
방식이다. 

*/

#include<assert.h>

template<typename T>
class CMergeSort
{
public :
	CMergeSort()
	{
		m_Size = 0;
		m_Capacity = 8;
		m_Array = new T[m_Capacity];
		m_CopyArray = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CMergeSort()
	{
		delete[] m_Array;
		delete[] m_CopyArray;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	T* m_CopyArray;
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
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];
			memcpy(Array, m_Array, sizeof(T) * m_Size);
			delete[] m_Array;
			delete[] m_CopyArray;
			m_Array = Array;
			m_CopyArray = new T[m_Capacity];
		}
		m_Array[m_Size] = data;
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
		MergeSort(0, Count - 1, m_Array);
	}
	void MergeSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			int Mid = (Left + Right) / 2;
			MergeSort(Left, Mid, Array);
			MergeSort(Mid + 1, Right, Array);
			Merge(Left, Mid, Right, Array);
		}
	}
	void Merge(int Left, int Mid, int Right, T* Array)
	{
		int Low = Left;
		int High = Mid+1;
		int Pivot = Left;
		while (Left <= Mid && High <= Right)
		{
			if (m_Func(m_Array[High],m_Array[Low]))
			{
				m_CopyArray[Pivot] = m_Array[Low];
				++Pivot;
				++Low;
			}
			else
			{
				m_CopyArray[Pivot] = m_Array[High];
				++Pivot;
				++High;
			}
		}
		if (Left <= Mid)
		{
			for (int i = Left; i <= Mid; i++)
			{
				m_CopyArray[Pivot] = m_Array[i];
				++Pivot;
			}
		}
		else
		{
			for (int i = High; i <= Right; i++)
			{
				m_CopyArray[Pivot] = m_Array[i];
				++Pivot;
			}
		}
		for (int i = Left; i <= Right; i++)
		{
			m_Array[i] = m_CopyArray[i];
		}
	}

};