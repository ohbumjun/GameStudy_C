#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// AddProcess.cpp
// - main 함수의 전달인자를 덧셈하는 프로그램
int _tmain(int argc, TCHAR* argv[])
{
	DWORD val1, val2;

	// __ttoi 함수 ?
	// - 문자열 데이터를 정수형 데이터로 변경시키는 atoi +
	// 유니코드 기반의 _wtoi 를 동시 지원하기 위한 매크로
	val1 = _ttoi(argv[1]);
	val2 = _ttoi(argv[2]);

	_tprintf(_T("%d + %d = %d \n"), val1, val2, val1 + val2);

	// 프로그램 실행을 잠시 멈추기 위해
	// getchar() 함수와 유니코드 기반의 getwchar() 함수를 동시 지원하기 위한 매크로
	_gettchar();

	return 0;
}