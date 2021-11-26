/*
문자셋의 종류 3가지
- Single Byte Charadter Set
문자 표현하는데 1바이트 사용 = 아스키 코드
- Multi Byte Character Set
한글은 2바이트, 영문은 1 바이트
- Wide Byte Character SEt
뮨자를 표현하는데 2바이트 사용
유니코드 
*/


// 유니코드 기반
// 1. char를 대신하는 wchar_t
// 2. "ABC"를 대신하는 L"ABC"
// 그냥 "ABC" --> 아스크 코드로 표현하겠다는 암묵적 의미

// ex
wchar_str str[] = L"ABC";

int main()
{
	wchar_t str[] = L"ABC";

	// 문자열을 다루는 방법과 관계없이
	// 할당된 메모리크기는 그대로
	// 즉, 영어는 1 바이트, 한글은 2바이트 로 메모리 크기를 할당한다는 것이다 
	int size = sizeof(str);

	// 문자열 길이를 반환하는 함수
	// strlen(char*) : char* 타입을 인자로 기대한다.
	//  wcslen(wchar_t* ) : wchar_t* 타입을 인자로 기대한다.

	// 문자열 길이 3으로 인식 
	// 한글 하나도 1개의 글자로 인식한다. 유니코드는 
	int len = wcslen(str);

	 // 아래는 아스키 코드 기반 처리
	printf("배열 크기 : %d", size);

	// 통일 위해서 유닛 코드 기반 함수로 출력하기
	wprintf(L"Array Size : ", size);

	return 0;
}

