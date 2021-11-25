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
		m_Func = SortFunction;
	}
	~CMergeSort()
	{
		delete[] m_Array;
	}
private :
	int m_Size;
	int m_Capacity;
	T* m_Array;
	T* m_CopyArray;
	bool (*pFunc)(const T&, const T&);
public :
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];
			memcpy(Array, m_Array, sizeof(T) * m_Size);
			delete[] m_Array;
			m_Array = Array;

			delete[] m_CopyArray;
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
		MergeSort(0, Count - 1, Array);
	}
	void MergeSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			int Mid = (Left + Right) / 2;
			MergeSort(Left, Mid, Array);
			MergeSort(Mid+1, Right, Array);
			Merge(Left, Mid, Right, Array);
		}
	}
	void Merge(int Left, int Mid, int Right, T* Array)
	{
		int Low = Left;
		int High = Mid + 1;
		int Pivot = Left;
		// 둘다 정렬 
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
		// 왼쪽 정렬이 끝났을 때
		if (Low > Mid)
		{
			// 오른쪽을 마저 정렬해준다
			for (int i = High; i <= Right; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				Pivot++;
			}
		}
		// 오른쪽 정렬이 끝났을 때
		else
		{
			// 왼쪽을 마저 정렬해준다
			for (int i = Low; i <= Mid; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				Pivot++;
			}
		}
		// 원래 배열에 복사해서 넣어준다
		for (int i = Left; i <= High; i++)
		{
			m_Array[i] = m_CopyArray[i];
		}
	}
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};