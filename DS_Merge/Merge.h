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
// copy본을 하나 만들고
// 그곳에다가 정렬된 요소를 넣어가는 과정
	// 따라서 copy본 반드시 필요
private :
	T* m_Array;
	T* m_CopyArray;
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
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];

			memcpy(Array, m_Array, sizeof(T) * m_Size);
			
			delete[] m_Array;
			delete[] m_CopyArray;
			
			m_CopyArray = new T[m_Capacity];
			m_Array = Array;
		}

		m_Array[m_Size] = data;
		++m_Size;
	}
	void push(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[]	m_Array;
			delete[]	m_CopyArray;
			m_Array = new T[m_Capacity];
			m_CopyArray = new T[m_Capacity];
		}

		for (int i = 0; i < Count; ++i)
		{
			m_Array[i] = Array[i];
		}

		m_Size = Count;
	}

	void SetSotFunction(bool (*pFunc)(const T& Left, const T& Right))
	{
		m_Func = pFunc;
	}
	int size() const { return m_Size; }
	bool empty() const { return m_Size == 0; }
	void Sort()
	{
		MergeSort(0, m_Size - 1, m_Array);
	}
	void Sort(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[]	m_Array;
			delete[]	m_CopyArray;
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
			// 자기 자신 계속 호출해서
			// 분할 먼저
			// 가운데 인덱스 구해주기
			int Mid = (Left + Right) / 2;

			// 왼쪽을 분할 한다
			MergeSort(Left, Mid, Array);
			// 오른쪽 분할 한다
			MergeSort(Mid + 1, Right, Array);

			// 분할된 순서로 
			// 되돌아가면서 정렬
			Merge(Left, Mid, Right, Array);

		}
	}
	
	void Merge(int Left, int Mid, int Right, T* Array)
	{
		// 되돌아가면서
		// 값을 비교하면서 정렬
		int Low = Left;
		int High = Mid + 1;
		int Pivot = Left;

		// ex) 1 3 5 7
		// low : 1, mid : 3, high : 5

		// Low 기준, High와 비교/
		// 왼쪽과 오른쪽 집단 비교

		// Low가 mide보다 커지면
		// 오른쪽 집단 요소를 가리키게 되므로
		// ㄴㄴ 이다 ! 
		while(Low <= Mid && High <= Right)
		{
			if (m_Func(Array[High], Array[Low]))
			{
				m_CopyArray[Pivot] = Array[Low];
				++Low;
				++Pivot; // pivot : 데이터를 담을 idx
			}
			else
			{
				m_CopyArray[Pivot] = Array[High];
				++High;
				++Pivot;
			}
		}

		// Low가 Mid보다 크다면
		// 왼쪽 배열 값이 모두
		// 처리가 되었다는 의미이다.
		if (Low > Mid)
		{
			for (int i = High; i <= Right; ++i)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}
		// 오른쪽 배열의 값이
		// 모두 처리가 되었다는 의미이다
		else
		{
			for (int i = Low; i <= Mid; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}

		// 복사본 배열에 정렬된 내용을
		// 원본 배열로 옮겨준다
		for (int i = Left; i <= Right; i++)
		{
			Array[i] = m_CopyArray[i];
		}
	}
	
};