#pragma once

#include <assert.h>

template <typename T, int SIZE = 200>
class CCircleQueue
{
public:
	CCircleQueue()
	{
		m_Size = 0;
		m_Head = 0;
		m_Tail = 0;
		m_Capacity = SIZE + 1;
	}

	~CCircleQueue()
	{
	}

private:
	T		m_Queue[SIZE + 1];
	int		m_Capacity;
	int		m_Size;
	int		m_Head;	// ���� ó�� �߰��� ���� ���� �ε���
	int		m_Tail;	// ���������� �߰��� ���� �ε���

public:
	void push(const T& data)
	{
		int	Tail = (m_Tail + 1) % m_Capacity;

		if (Tail == m_Head)
			return;

		m_Queue[Tail] = data;

		m_Tail = Tail;

		++m_Size;
	}

	T& front()
	{
		if (empty())
			assert(false);

		int	Head = (m_Head + 1) % m_Capacity;

		return m_Queue[Head];
	}

	void pop()
	{
		if (empty())
			assert(false);

		m_Head = (m_Head + 1) % m_Capacity;

		--m_Size;
	}

	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	void clear()
	{
		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;
	}
};

