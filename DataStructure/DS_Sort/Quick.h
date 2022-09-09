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

			// memcpy : �޸� ���� �Լ� 
			memcpy(Array, m_Data, sizeof(T) * m_Size);
			delete[] m_Data;

			m_Data = Array;
		}
		m_Data[m_Size] = data; // ��������� array�͵���

		// �������� �߰��ϸ鼭 ����
		// �������� �߰��� �ϰ�, �� ���� �Ѳ����� ���� 

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

	// ������ �����Լ��� �����Ѵ�. 
	void Sort()
	{
		QuickSort(0, m_Size - 1, m_Data);
	}
	void Sort(T* Array, int Count)
	{
		// �ܺι迭 �ٷ� ����
		QuickSort(0, Count - 1, Array);
	}
private:
	void QuickSort(int Left, int Right, T* Array)
	{
		// pivot ��������,��,�� ����
		if (Left < Right)
		{
			// pivot ����
			int Pivot = Partition(Left, Right, Array);
			// left ~ pivot -1 , pivot + 1 ~ right
			
			// �Ǻ� ��������
			// ����, �������� �����ؼ� ����
			QuickSort(Left, Pivot - 1, Array);
			QuickSort(Pivot+1, right, Array);
		}
	}
	int Partition(int Left, int Right, T* Array)
	{
		int Low = Left;
		// Right�� idx�ε��� �� +1  ?
		// Right�� �������� ����... ?
		int High = Right + 1;

		// ���� ���ʰ��� ���ذ����� ����Ѵ�
		T Value = Array[Left];


		// �ٲ� low, high�� ã�� 
		do
		{
			do
			{

				// ���ڸ��� Low�� 1����
				// ��, ó�� ���ذ����� Array[Left]�� ������
				// Low = LEft // Low ++ �����Ƿ�
				// �ٸ������� �� ���� 
				++Low;

			// ���ذ��̶� , Array�� low ��
			// 
			}while(Low <= Right && m_Func(Value, Array[Low]))
		
			// ���⼭�� High �� ���� ó��
			do
			{
				// ó�� ���� ������
				// �� ������ data idx�� ����Ų��.
				--High;
			}while(High >= Left && m_Func(Array[High],Value))
		
				if (Low > High)
				{
					T Temp = Array[Low];
					Array[Low] = Array[High];
					Array[High] = Temp;
			}

		} while (Low < High)
			// pivot�� ��ü
			// High ��°���� pivot �� ����.
			T temp = Array[Left];
		Array[Left] = Array[High];
		Array[High] = Temp;
		return High;
	}
private :
	static bool SortFunction(const T& Left, const T& Right) //
	{
		return Left > Right;
	}
};
