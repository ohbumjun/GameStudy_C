#pragma once

template<typename T>
class CMergeSort {
public :
	CMergeSort()
{
		m_Func = SortFunction;

		m_Size = 0;
		m_Capacity = 8;

		m_Array = new T[m_Capacity];
		m_CopyArray = new T[m_Capacity];
}
	~CMergeSort(){}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	T* m_CopyArray;
	bool (*m_Func)(const T& D1, const T& D2);
public :
	int size() const
{
		return m_Size;
}
	int capacity() const
{
		return m_Capacity;
}
	void push(const T& Data)
{
	if (m_Size >= m_Capacity)
	{
		m_Capacity *= 2;
		T* NewArray = new T[m_Capacity];
		memcpy(NewArray, m_Array, sizeof(m_Array));

		delete [] m_Array;
		delete [] m_CopyArray;

		m_Array = NewArray;
		m_CopyArray = new T[m_Capacity];
	}

	m_Array[m_Size] = Data;

	++m_Size;
}
	void push(const T* Array, int Count)
{
	if (m_Capacity < Count)
	{
		m_Capacity = Count;

		delete [] m_Array;
		delete [] m_CopyArray;

		m_Array = new T[m_Capacity];
		m_CopyArray = new T[m_Capacity];
	}

	for (int i = 0; i < Count; ++i)
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
	// �ϳ��� ���� ������ ��� �����ϱ�
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
	int Pivot = Left;
	int Low = Left;
	int High = Mid + 1;

	while (Low <= Mid && High <= Right)
	{
		if (m_Func(m_Array[Low], m_Array[High])) ////
		{
			m_CopyArray[Pivot] = m_Array[Low];
			++Low;
		}
		else
		{
			m_CopyArray[Pivot] = m_Array[High];
			++High;
		}
		++Pivot;
	}

	// ���� ��� ����
	if (Low > Mid)
	{
		for (int i = High; i <= Right; ++i)
		{
			m_CopyArray[Pivot] = m_Array[i];
			++Pivot;
		}
	}

	// ������ ��� ����
	if (High > Right)
	{
		for (int i = Low; i <= Mid; ++i)
		{
			m_CopyArray[Pivot] = m_Array[i];
			++Pivot;
		}
	}

	for (int i = Left; i <= Right; ++i)
	{
		m_Array[i] = m_CopyArray[i];
	}
}
public :
	static bool SortFunction(const T& D1, const T& D2)
{
		return D1 < D2;
}
};