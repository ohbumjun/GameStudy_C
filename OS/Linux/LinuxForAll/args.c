#include <stdio.h>

// "x y z" 를 하면, argc 는 '2' 가 된다.
// 즉, xyz 가 하나의 인자로 인식되는 것이다
// "" 는 감싼 부분을 하나의 실행 인자로 묶어주는 효과가 있다.
int main(int argc, char* argv[])
{
	int i;

	printf("argc = %d\n", argc);

	for(int i = 0; i < argc; ++i)
	{
		printf("argv[%d]=%s\n", i, argv[i]);
	}

	return 0;
}
