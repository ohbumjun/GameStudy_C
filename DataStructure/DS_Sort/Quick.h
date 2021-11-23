#pragma once

#include<assert.h>

template<typename T>
class CQuickSort
{
public :
	CQuickSort()
	{

	}
	~CQuickSort()
	{

	}
private : 
	T* m_Data;
	int m_Size;
	int m_Capacity;
	bool (*m_Func)(const T&, const T&);
public :

public :
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];

			// memcpy : 메모리 복사 함수 
			memcpy(Array, m_Data, sizeof(T) * m_Size);
			delete[] m_Data;

			m_Data = Array;
		}
		m_Data[m_Size] = data; // 여기까지는 array와동일

		// 힙정렬은 추가하면서 정렬
		// 퀵정렬은 추가다 하고, 그 다음 한꺼번에 정렬 

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
		for (int i = 0; i < Count; i++)
		{
			m_Data[i] = Array[i];
		}
		m_Size = Count;
	}
	int size() const
	{
		return m_Size;
	}
	bool empty() const
	{
		return m_Size == 0;
	}

	// 별도의 정렬함수를 마련한다. 
	void Sort()
	{
		QuickSort(0, m_Size - 1, m_Data);
	}
	void Sort(T* Array, int Count)
	{
		// 외부배열 바로 정렬
		QuickSort(0, Count - 1, Array);
	}
private:
	void QuickSort(int Left, int Right, T* Array)
	{
		// pivot 기준으로,왼,오 분할
		if (Left < Right)
		{
			// pivot 세팅
			int Pivot = Partition(Left, Right, Array);
			// left ~ pivot -1 , pivot + 1 ~ right
			
			// 피봇 기준으로
			// 왼쪽, 오른쪽을 분할해서 들어간다
			QuickSort(Left, Pivot - 1, Array);
			QuickSort(Pivot+1, right, Array);
		}
	}
	int Partition(int Left, int Right, T* Array)
	{
		int Low = Left;
		// Right가 idx인데도 왜 +1  ?
		// Right는 데이터의 개수... ?
		int High = Right + 1;

		// 가장 왼쪽값을 기준값으로 사용한다
		T Value = Array[Left];


		// 바꿀 low, high를 찾기 
		do
		{
			do
			{

				// 들어가자마자 Low를 1증가
				// 즉, 처음 기준값으로 Array[Left]로 했으나
				// Low = LEft // Low ++ 했으므로
				// 다른값끼리 비교 시작 
				++Low;

			// 기준값이랑 , Array의 low 비교
			// 
			}while(Low <= Right && m_Func(Value, Array[Low]))
		
			// 여기서는 High 에 대해 처리
			do
			{
				// 처음 여기 들어오면
				// 맨 마지막 data idx를 가리킨다.
				--High;
			}while(High >= Left && m_Func(Array[High],Value))
		
				if (Low > High)
				{
					T Temp = Array[Low];
					Array[Low] = Array[High];
					Array[High] = Temp;
			}

		} while (Low < High)
			// pivot과 교체
			// High 번째꺼에 pivot 이 들어간다.
			T temp = Array[Left];
		Array[Left] = Array[High];
		Array[High] = Temp;
		return High;
	}
private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};
