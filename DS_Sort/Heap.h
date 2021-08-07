#pragma once
#include<assert.h>

template<typename T>
class CHeap
{
	// 배열 기반 구현
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
		// 정렬할 때는, 항상 함수포인터를 인자로 받아서
		// 해당 함수포인터를 통해 return되는 것에 따라
		// 최소,최대힙이 될 지를 결정할 것이다
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

			// memcpy : 메모리 복사 함수 
			memcpy(Array, m_Data, sizeof(T) * m_Size);
			delete[] m_Data;

			m_Data = Array;
		}
		m_Data[m_Size] = data; // 여기까지는 array와동일

		// 힙으로 구성한다
		InsertHeap(m_Size);

		++m_Size;
	}
private :
	void InsertHeap(int Index)
	{
		// Index가 0이면 루트노드라는 의미이다.
		// 그러므로, 더이상 작업을 안하고 
		// 반환한다. 
		// 왜냐하면, 맨 밑에다 추가하고
		// 맨 위부터 비교해야 하는데
		// 애초에 기존에 아무것도 없다는 것이다 
		// 따라서 x 
		// ( 재귀의 개념 )
		if (Index == 0) return;

		// 부모노드의 idx를 구해준다
		// idx - 1 // 2
		int ParentIndex = (Index - 1) / 2;

		if (m_Func(m_Data[ParentIndex], m_Data[Index]))
		{
			T temp = m_Data[ParentIndex];
			m_Data[ParentIndex] = m_Data[Index];
			m_Data[Index] = temp;;
			// 맨 아래다가 추가후 
			InsertHeap(ParentIndex);
		}
	}

	void DeleteHeap(int Index)
	{
		// 삭제 시 반씩 날리면서 작업한다
		// 왼쪽, 오른쪽 자식 idx를 구하여 비교
		// []*2 + 1 
		// []*2

		// 왼쪽 자식노드의 인덱스
		int LeftChildIndex = Index * 2 + 1;
		if (LeftChildIndex >= m_Size)
		{
			// 끝났다는 의미
			// 더이상 자식노드가 없다는 것
			// 왜 ? 왼쪽없으면, 오른쪽도 x
			// 순차적으로 왼쪽부터 추가해가므로
			return;
		}
		// 오른쪽 자식노드 idx
		int RightChildIndex = LeftChildIndex + 1;

		// defaul 세팅 
		int ChildIndex = LeftChildIndex;

		// 오른쪽 자식노드 있는지 판다
		if (RightChildIndex < m_Size)
		{
			// 오른쪽 자식 노드 있을 경우
			// 왼,오 비교하여 선택한다.
			if (m_Func(m_Data[LeftChildIndex], m_Data[RightChildIndex]))
			{
				// 오른쪽 자식노드 
				// 가 비교대상이 된다는 것 
				ChildIndex = RightChildIndex;
			}
		}
		// 최종 선택된 childIndex와 값을 비교
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

		// 하나남으면, 그것만 지워주기
		else if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}
		--m_Size;

		// 위에서 빼고
		// 가장 마지막에 추가된 노드를
		// 루트로 올린다( 오른쪽 아래 )
		m_Data[0] = m_Data[m_Size];
		DeleteHeap(0);
	}


private :
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

