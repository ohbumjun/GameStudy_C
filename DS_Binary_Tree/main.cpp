#include<iostream>
#include<time.h>
#include "CBianaryTree.h"

using namespace std;

int Factorial(int Number)
{
	// 함수가 종료가 되어야만
	// 해당 함수로 인해 만들어진
	// Stack 내 메모리들이 사라진다.

	// 그런데, 함수 종료 안되고
	// 재귀적으로 진행하게 되면 
	// 계속 stack에 지역변수 목록들이 쌓이게 된다. 
	if (Number == 1) return 1;
	return Number * Factorial(Number - 1);
}

// 꼬리 재귀
// : 최적화된 재귀함수 만들기
// 지역변수가 stack에 쌓이는 것까지는
// 어떻게 할 도리가 없다. 변수는 만들어야 하니까
// 단, 
// 
int FactorialTail(int Number, int Result)
{
	if (Number == 1) return Result;
	// 인자에다가 계산한 결과를 넣어주는 것
	// 이렇게 되면, 자체 함수가 재귀적으로 다시 호출될 때
	// 별도의 변수를 생성하는 작업을 거치지 않는다
	// 임시변수 만들고 + 계산하고 + 리턴하고 
	// ex) return Number + Factorial(Number - 1);
	// 이런 과정을 거치는 게 아니라
	// 인자에다가 결과값을 넣어주기만 하는 것이다 
	return FactorialTail(Number - 1, Result * Number);
}

int FactorialTail(int Number)
{
	return FactorialTail(Number,1);
}


int main()
{
	srand((unsigned int)time(0));
	rand();

	CBinaryTree<int, int> tree;

	tree.insert(333, 333);
	for (int i = 0; i < 20; i++)
	{
		int Number = rand();
		tree.insert(Number, Number);
	}

	CBinaryTree<int, int>::iterator iter;

	for (iter = tree.begin(); iter != tree.end(); ++iter)
	{
		// iter-> 를 하는 순간 m_Node
		// 그러면 iter->()->first 이런 식으로 화살표 한번 더 사용해야 하는 거 아닌가 ?
		// 맞는데, 여기서 ->가 한번 생략되면서 위와 같은 operator가 작동한다고 생각하면 편한다 
		cout << "key : " << iter->first << endl;
		cout << "value : " << iter->second << endl;
	}

	cout << Factorial(5) << endl;
	cout << FactorialTail(5) << endl;
	
	iter = tree.Find(333);

	if (iter == tree.end())
		cout << "찾는 데이터가 없습니다." << endl;
	else
	{
		cout << "Find Key :" << iter->first  << endl;
		cout << "Find Value :" << iter->second << endl;
	}

	return 0;
}

/*
< 트리 >
>> 기본 개념
부모 노드 존재
- 이 녀석의 자식 노드들이 존재
- 자식의 자식도 존재 ... 뻗어나가는 과정

>> 루트 노드 : 제일 꼭대기 노드

>> 서브트리

>> 리프 노드
: 더이상 자식노드가 존재 x

--------------------------------
< 이진 트리 >
>> 기본 개념
- 자식 노드가 최대 2개까지만 존재
- 작으면 왼쪽, 크면 오른쪽

>> 데이터 탐색에 있어서 최적화

>> 1) 포화 이진 트리
- 모든 공간이 다 차있는 형태

>> 2) 완전 이진 트리
- 1,2개 정도 빠져있으면
완전 이진 트리

>> 3) 편향 트리
- 한쪽으로만 치우쳐져 있음
- 정렬된 데이터가 차례대로 들어가게 되면
편향트리가 만들어진다
ex) 1234567 --> :: 1->2->3 .. ->7
(참고) 자가 균형 이진트리
: 데이터를 넣을 때마다,
알아서 이진트리 균형을 맞춰준다 .

>> 처음 아무것도 없는 이진트리에
데이터를 추가하면, 해당 값은
루트노드에 들어간다.
- 이후, 만나는 값보다 큰 값을 넣으면
해당 노드 오른쪽
- 작은 값을 넣으면, 해당 노드 왼쪽

*/