#include<iostream>
#include "Heap.h"
#include<time.h>

// 힙 
// 최소힙 : 오름차순 -- 최대힙 : 내림차순 
// 이진트리를 구성하는 방법이 달라진다
// 이진트리는 참고로, 배열로도 구성할 수 있다
// [Root][Root_L][Root-R][Root-_LL][Root_LR][Root_RL][Root_RR]

// 힙정렬은, 여기서는 배열기반. 구성할 것이다
// ( 최대 힙 )
// 특징 : 제일 마지막에 집어넣는다. push_back 같은 것

// 트리상으로는, 마치 아래에다가 달아주는 느낌
// 왼쪽에서 오른쪽 , 즉, 왼쪽 아래부터 빈공간 보면서
// 달아주는 형태이다. 

// 부모보다 추가한 애가 더 크면, 위치를 바꾼다
// 제일 큰애가 루트쪽으로 위치하는 형태 

// 그렇다면, 최대힙에서 "삭제"를 한다면 ??
// "루트" 에 위치해 있는,
// 가장 큰.애가 없어지고
// 가정 오른쪽 아래에 위치한 애를 
// 해당 루트 노드에 올리고

// 차후 자식 노드들과 비교해간다
// 왼쪽,오른쪽 자식이 둘다 크다면 
// 더 큰 자식과 바꿔주는 형태이다. 
/// (최대힙.에서는 큰애를 위로 올린다)

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	rand();

	CHeap<int> heap;
	Quick.SetSortFunction(Sort);

	int Array[20] = {};

	for (int i = 0; i < 20; i++)
	{
		Array[i] = rand();
		std::cout << << std::endl;
	}

	for (int i = 0; i < 20; i++)
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