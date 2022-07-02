#pragma once

// Heap : 완전 이진트리를 기초로 하는 자료구조
// - 우선 순위 큐를 구현하기 위해 많이 사용된다.
#include <assert.h>

// 최대힙
template<typename T>
class CHeap {
public :
	CHeap()
{
	m_Size = 0;
	m_Capacity = 8;
	m_Array = new T[m_Capacity];
}
	~CHeap()
{
		delete[] m_Array;
}
private :
	T* m_Array;
	int m_Size;
	int m_Capacity;
public :
	void push(const T& Data)
{
	if (m_Size >= m_Capacity)
	{
		m_Capacity *= 2;
		T* NewArray = new T[m_Capacity];
		memcpy(m_Array, NewArray);
		delete[] m_Array;
		m_Array = NewArray;
	}
	m_Array[m_Size] = Data;
	InsertHeap(m_Size);
	++m_Size;
}
	void pop()
{
	if (empty())
		assert(false);

	--m_Size;
	m_Array[0] = m_Array[m_Size];
	DeleteHeap(0);
}
	void clear()
{
		m_Size = 0;
}
	T& top() const
{
	if (empty())
		assert(false);

		return m_Array[0];
}
	bool empty() const
{
		return m_Size == 0;
}
	int size() const
{
		return m_Size;
}
	int capacity() const
{
		return m_Capacity;
}
private :
	// 맨끝에 Data 를 insert 한 이후, 위로 올라가면서 제자리를 찾아가게 한다.
	void InsertHeap(int Index)
{
	if (Index == 0)
		return;

	int ParentIndex = (Index - 1) / 2;

	if (m_Array[Index] > m_Array[ParentIndex])
	{
		T temp = m_Array[Index];
		m_Array[Index] = m_Array[ParentIndex];
		m_Array[ParentIndex] = temp;
		InsertHeap(ParentIndex);
	}
}
	// 맨 처음 Root 를 지우고, 가장 끝에 있는 Data 를 Root 위치에 둔 다음
	// 내려가면서 제자리를 찾아가게 한다.
	void DeleteHeap(int Index)
{
	// 왼쪽, 오른쪽 자식 둘중에서 더 큰값이랑 바꿔야 한다.
		int LeftChildIndex = (Index * 2) + 1;
		int ChildIndex = LeftChildIndex;

		if (LeftChildIndex >= m_Size)
			return;

		int RightChildIndex = LeftChildIndex + 1;

		if (RightChildIndex < m_Size && m_Array[RightChildIndex] > m_Array[LeftChildIndex])
			ChildIndex = RightChildIndex;

		T temp = m_Array[ChildIndex];
		m_Array[ChildIndex] = m_Array[Index];
		m_Array[Index] = temp;
		DeleteHeap(ChildIndex);
}
};

/*
int main()
{
	CHeap<int>	heap;

	for (int i = 0; i < 20; ++i)
	{
		heap.push(rand());
	}
	while (!heap.empty())
	{
		std::cout << heap.top() << std::endl;
		heap.pop();
	}

	return 0;
}
 */