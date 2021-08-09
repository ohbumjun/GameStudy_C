#pragma once


/*
���������� ��� ,
�迭 �ϳ��� �߰�������
������ �Ѵ�.

���������� �ݾ� �ɰ��� �������
�����ϸ� �ȴ�.

3 7 1 5 6 8 2 4

3 7 1 5			6 8 2 4

3 7		1 5		6 8		2 4

3	7	1	5	6	8	2	4

��. �������� ����.
�ٽ� �������� �������� ��ģ��

37	15	68	24

37 ~ 15�� ���Ѵ�
1 3 5 7	    2 4 6 8 

1 2 3 4 5 6 7 8

��, �켱 1�� ������ ���� ������,
�ǵ��ƿ��鼭, ������ �����ϴ�
����̴�. 

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
// copy���� �ϳ� �����
// �װ����ٰ� ���ĵ� ��Ҹ� �־�� ����
	// ���� copy�� �ݵ�� �ʿ�
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
			// �ڱ� �ڽ� ��� ȣ���ؼ�
			// ���� ����
			// ��� �ε��� �����ֱ�
			int Mid = (Left + Right) / 2;

			// ������ ���� �Ѵ�
			MergeSort(Left, Mid, Array);
			// ������ ���� �Ѵ�
			MergeSort(Mid + 1, Right, Array);

			// ���ҵ� ������ 
			// �ǵ��ư��鼭 ����
			Merge(Left, Mid, Right, Array);

		}
	}
	
	void Merge(int Left, int Mid, int Right, T* Array)
	{
		// �ǵ��ư��鼭
		// ���� ���ϸ鼭 ����
		int Low = Left;
		int High = Mid + 1;
		int Pivot = Left;

		// ex) 1 3 5 7
		// low : 1, mid : 3, high : 5

		// Low ����, High�� ��/
		// ���ʰ� ������ ���� ��

		// Low�� mide���� Ŀ����
		// ������ ���� ��Ҹ� ����Ű�� �ǹǷ�
		// ���� �̴� ! 
		while(Low <= Mid && High <= Right)
		{
			if (m_Func(Array[High], Array[Low]))
			{
				m_CopyArray[Pivot] = Array[Low];
				++Low;
				++Pivot; // pivot : �����͸� ���� idx
			}
			else
			{
				m_CopyArray[Pivot] = Array[High];
				++High;
				++Pivot;
			}
		}

		// Low�� Mid���� ũ�ٸ�
		// ���� �迭 ���� ���
		// ó���� �Ǿ��ٴ� �ǹ��̴�.
		if (Low > Mid)
		{
			for (int i = High; i <= Right; ++i)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}
		// ������ �迭�� ����
		// ��� ó���� �Ǿ��ٴ� �ǹ��̴�
		else
		{
			for (int i = Low; i <= Mid; i++)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}

		// ���纻 �迭�� ���ĵ� ������
		// ���� �迭�� �Ű��ش�
		for (int i = Left; i <= Right; i++)
		{
			Array[i] = m_CopyArray[i];
		}
	}
	
};