#pragma once

#include<assert.h>

// B Type Template
template<typename T, int SIZE = 200>
class CCircleQueue
{
	// 반드시 공간 하나는
	// head,tail이 만나는 공간이 필요하다
	// 기본원리 : head가 tail을 따라가는 구조
public :
	CCircleQueue()
	{
		m_Size = 0;
		
		// 처음 Head,Tail은 같은 공간 가리킨다.
		// 가장 처음 추가된 곳의 이전 idx 
		m_Head = 0;
		// 마지막으로 추가된 곳의 idx
		m_Tail = 0;
		// + 1 : 빈공간 1개를 남기는 것
		// 즉, 선언한 size - 1개 만큼 존재하는 것 
		m_Capacity = SIZE + 1;
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
		// Tail 쪽으로 !
		// 그런데, 만일 Tail이 가장 끝에라면
		// 여기서 +1 끝을 벗어나므로
		// 앞으로 돌아가야 한다
		// 그런데 앞으로 돌아갔더니
		// head ? 꽉찬 것
		// 만약, head가 아니라면 
		// 공간이 있어서 넣을 수 있다는 의미
		// how ? 나머지 연산 

		// ex) 11개 크기 --> 마지막 idx : 10
		// Head,Tail 둘다 처음에 0으로 선언 
		// Head, Tail 이 같다면, 빈공간인 것
		// Head가 가리키는 곳은 항상 빈공간 
		int Tail = (m_Tail + 1) % m_Capacity;
		
		// 꽉 찼다면 
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
		// 왜 +1  ?
		// 처음에는 0으로 되어 있기 때문에
		int Head = (m_Head + 1) % m_Capacity;
		return m_Queue[m_Head];
	}
	void pop()
	{
		if (empty())
			assert(false);
		m_Head = (m_Head + 1) % m_Capacity;
		// Head가 Tail을 쫒아가는 형태 
		--m_Size;
	}
	int size() const
	{
		return m_Size;
	}
	bool empty() const
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

