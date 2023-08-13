#pragma once

#include <assert.h>

template <typename T>
class CHeap
{
public:
	CHeap()
	{
		m_Capacity = 8;
		m_Size = 0;

		m_Data = new T[m_Capacity];

		m_Func = SortFunction;
	}

	~CHeap()
	{
		delete[]	m_Data;
	}

private:
	T* m_Data;
	int	m_Size;
	int	m_Capacity;
	bool (*m_Func)(const T&, const T&);

public:
	void SetSortFunction(bool (*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}

public:
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];

			memcpy(Array, m_Data, sizeof(T) * m_Size);

			delete[]	m_Data;

			m_Data = Array;
		}

		m_Data[m_Size] = data;

		// ������ �����Ѵ�.
		InsertHeap(m_Size);

		++m_Size;
	}

	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	T top()	const
	{
		if (empty())
			assert(false);

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

		else if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}

		--m_Size;

		// ���� �������� �߰��� ��带 ��Ʈ�� �ø���.
		m_Data[0] = m_Data[m_Size];

		DeleteHeap(0);
	}

private:
	void InsertHeap(int Index)
	{
		// Index�� 0�̸� ��Ʈ����� �ǹ��̴�. �׷��Ƿ� ���̻� �۾��� ���ϰ� ��ȯ�Ѵ�.
		if (Index == 0)
			return;

		// �θ����� �ε����� �����ش�.
		int	ParentIndex = (Index - 1) / 2;

		if (m_Func(m_Data[ParentIndex], m_Data[Index]))
		{
			T	temp = m_Data[ParentIndex];
			m_Data[ParentIndex] = m_Data[Index];
			m_Data[Index] = temp;

			InsertHeap(ParentIndex);
		}
	}

	void DeleteHeap(int Index)
	{
		// ���� �ڽĳ���� �ε����� �����ش�.
		int	LeftChildIndex = Index * 2 + 1;

		// m_Size���� ũ�ų� ���ٸ� ���̻� �ڽĳ�尡 ���ٴ� �ǹ��̴�.
		if (LeftChildIndex >= m_Size)
			return;

		// ������ �ڽĳ���� �ε����� ���Ѵ�.
		int	RightChildIndex = LeftChildIndex + 1;

		int	ChildIndex = LeftChildIndex;

		// ������ �ڽĳ�尡 �ִ��� �Ǵ��Ѵ�.
		if (RightChildIndex < m_Size)
		{
			// ������ �ڽĳ�尡 ���� ��� ���ʰ� �������� ���� ���Ͽ� �������ش�.
			if (m_Func(m_Data[LeftChildIndex], m_Data[RightChildIndex]))
				ChildIndex = RightChildIndex;
		}

		// ���� ���õ� ChildIndex�� ���� ���Ѵ�.
		if (m_Func(m_Data[Index], m_Data[ChildIndex]))
		{
			T	temp = m_Data[Index];
			m_Data[Index] = m_Data[ChildIndex];
			m_Data[ChildIndex] = temp;

			DeleteHeap(ChildIndex); //
		}
	}

private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

