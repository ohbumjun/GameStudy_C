#pragma once
#include<assert.h>

template<typename T>
class CHeap
{
	// �迭 ��� ����
public :
	CHeap()
	{
		m_Size = 0;
		m_Capacity = 8;

		m_Data = new T[m_Capacity];
		m_Func = SortFunction;
	}
	~CHeap()
	{
		delete[] m_Data;
	}
private : 
	T* m_Data;
	int m_Size;
	int m_Capacity;
		// ������ ����, �׻� �Լ������͸� ���ڷ� �޾Ƽ�
		// �ش� �Լ������͸� ���� return�Ǵ� �Ϳ� ����
		// �ּ�,�ִ����� �� ���� ������ ���̴�
	bool (*pFunc)(const T&, const T&);
public :
	void Set
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

		// ������ �����Ѵ�
		InsertHeap(m_Size);

		++m_Size;
	}
private :
	void InsertHeap(int Index)
	{
		// Index�� 0�̸� ��Ʈ����� �ǹ��̴�.
		// �׷��Ƿ�, ���̻� �۾��� ���ϰ� 
		// ��ȯ�Ѵ�. 
		// �ֳ��ϸ�, �� �ؿ��� �߰��ϰ�
		// �� ������ ���ؾ� �ϴµ�
		// ���ʿ� ������ �ƹ��͵� ���ٴ� ���̴� 
		// ���� x 
		// ( ����� ���� )
		if (Index == 0) return;

		// �θ����� idx�� �����ش�
		// idx - 1 // 2
		int ParentIndex = (Index - 1) / 2;

		if (m_Func(m_Data[ParentIndex], m_Data[Index]))
		{
			T temp = m_Data[ParentIndex];
			m_Data[ParentIndex] = m_Data[Index];
			m_Data[Index] = temp;;
			// �� �Ʒ��ٰ� �߰��� 
			InsertHeap(ParentIndex);
		}
	}

	void DeleteHeap(int Index)
	{
		// ���� �� �ݾ� �����鼭 �۾��Ѵ�
		// ����, ������ �ڽ� idx�� ���Ͽ� ��
		// []*2 + 1 
		// []*2

		// ���� �ڽĳ���� �ε���
		int LeftChildIndex = Index * 2 + 1;
		if (LeftChildIndex >= m_Size)
		{
			// �����ٴ� �ǹ�
			// ���̻� �ڽĳ�尡 ���ٴ� ��
			// �� ? ���ʾ�����, �����ʵ� x
			// ���������� ���ʺ��� �߰��ذ��Ƿ�
			return;
		}
		// ������ �ڽĳ�� idx
		int RightChildIndex = LeftChildIndex + 1;

		// defaul ���� 
		int ChildIndex = LeftChildIndex;

		// ������ �ڽĳ�� �ִ��� �Ǵ�
		if (RightChildIndex < m_Size)
		{
			// ������ �ڽ� ��� ���� ���
			// ��,�� ���Ͽ� �����Ѵ�.
			if (m_Func(m_Data[LeftChildIndex], m_Data[RightChildIndex]))
			{
				// ������ �ڽĳ�� 
				// �� �񱳴���� �ȴٴ� �� 
				ChildIndex = RightChildIndex;
			}
		}
		// ���� ���õ� childIndex�� ���� ��
		if (m_Func())
		{
			T Temp = m_Data[Index];
			m_Data[Index] = m_Data[Child]
		}

	}

	int size() const
	{
		return m_Size;
	}
	bool empty() const
	{
		return m_Size == 0;
	}
	T top() const
	{
		if (empty()) assert(false);
		return m_Data[0];
	}
	void clear()
	{
		m_Size = 0;
	}
	void pop()
	{
		if (empty())
			assert(false);

		// �ϳ�������, �װ͸� �����ֱ�
		else if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}
		--m_Size;

		// ������ ����
		// ���� �������� �߰��� ��带
		// ��Ʈ�� �ø���( ������ �Ʒ� )
		m_Data[0] = m_Data[m_Size];
		DeleteHeap(0);
	}


private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

