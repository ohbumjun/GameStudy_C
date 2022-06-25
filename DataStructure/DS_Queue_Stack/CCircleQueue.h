#pragma once

#include<assert.h>

// B Type Template
template<typename T, int SIZE = 200>
class CCircleQueue
{
	// �ݵ�� ���� �ϳ���
	// head,tail�� ������ ������ �ʿ��ϴ�
	// �⺻���� : head�� tail�� ���󰡴� ����
public :
	CCircleQueue()
	{
		m_Size = 0;
		
		// ó�� Head,Tail�� ���� ���� ����Ų��.
		// ���� ó�� �߰��� ���� ���� idx 
		m_Head = 0;
		// ���������� �߰��� ���� idx
		m_Tail = 0;
		// + 1 : ����� 1���� ����� ��
		// ��, ������ size - 1�� ��ŭ �����ϴ� �� 
		m_Capacity = SIZE + 1; //
	}
	~CCircleQueue()
	{

	}
private :
	T m_Queue[SIZE+1];
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;
public :
	void push(const T& data)
	{
		// Tail ������ !
		// �׷���, ���� Tail�� ���� �������
		// ���⼭ +1 ���� ����Ƿ�
		// ������ ���ư��� �Ѵ�
		// �׷��� ������ ���ư�����
		// head ? ���� ��
		// ����, head�� �ƴ϶�� 
		// ������ �־ ���� �� �ִٴ� �ǹ�
		// how ? ������ ���� 

		// ex) 11�� ũ�� --> ������ idx : 10
		// Head,Tail �Ѵ� ó���� 0���� ���� 
		// Head, Tail �� ���ٸ�, ������� ��
		// Head�� ����Ű�� ���� �׻� ����� 
		int Tail = (m_Tail + 1) % m_Capacity;
		
		// �� á�ٸ� 
		if (Tail == m_Head)
			return;

		m_Queue[Tail] = data;
		m_Tail = Tail;
		++m_Size;

	}
	T& front() const
	{
		if (empty())
			assert(false);
		// �� +1  ?
		// ó������ 0���� �Ǿ� �ֱ� ������
		int Head = (m_Head + 1) % m_Capacity;
		return m_Queue[Head];
	}
	void pop()
	{
		if (empty())
			assert(false);
		m_Head = (m_Head + 1) % m_Capacity;
		// Head�� Tail�� �i�ư��� ���� 
		--m_Size;
	}
	int size() const
	{
		return m_Size;
	}
	bool empty() const
	{
		return m_Size == 0; //
	}
	void clear()
	{
		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;

	}

};

