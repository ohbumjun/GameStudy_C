#include<iostream>

/*
해쉬는, ----------------
탐색 속도가 가장 빠른
알고리즘

HashTable
- 실제 저장되는 데이터보다,
훨씬 큰 메모리 공간을 만들어 놓고
데이터를 Hash 함수에 의해
인덱스를 구한다음
저장하는 방식이다.

실제 데이터가 저장될 때, 
연속으로 저장되는 것이 아니라,

배열에 빈공간들을 두고
저장이 되는 방식이다.

즉, 메모리가 낭비되는 것은
막을 수 없지만, 메모리를
희생해서, 
탐색속도를 극대화시키는 알고리즘이다.

Hash 충돌 --------------
- 중복된 idx에 데이터를
저장하려고 하는 경우 

Hash에서 중요한 것은
1) 공간이 넉넉한지
2) 좋은 Hash함수를
사용하고 있는것인지 

*/

int main()
{
	CHashTable<const char*, const char*> Table;

	Table.insert("야스로", "과학");
	Table.insert("요네", "수학");
	Table.insert("티모", "나쁜놈");
	Table.insert("트린다미어", "이기주의자");
	Table.insert("베민", "지만아는놈 ");

	std::cout << Table("야스오") << std::endl;


	return 0;
}