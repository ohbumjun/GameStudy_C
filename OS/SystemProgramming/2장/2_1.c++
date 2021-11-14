/*
Q1. 문자셋의 종류와 특성 3가지 

1. Single Byte Character Set
- 문자를 표현하는데 1 바이트 사용
- 아스키 코드

2. Multi Byte Character Set
- 한글은 2 바이트, 영문은 1 바이트 사용

3. Wide Byte Character Set
- 문자를 표현하는데 2바이트 사용
- 유니코드 

*/

// 멀티 바이트
int main()
{
    char str[] = "ABC한글";

    // 메모리 크기 
    // 영어 3, 한글 2*2, 마지막 null
    // 총 8 
    printf(sizeof(str))

    // 문자열 길이 : 7 --> 응 ? 5가 아닌건가 
    // 한글 각각이 문자열 길이 2씩 인식이 된 것이다 
    printf(strlen(str));

    return 0;
}

int main()
{
    char str[] = "한글입니다";
    int i;

    // 결과 : "한글"
    // 왜 ? 각각의 글자를 2byte 단위로 인식했기 때문이다
    for(int = 0 ; i < 5; i++)
        fputc(str[i], stdout);

    return 0;
}


// 유티코드 기반 
// 1. char를 대신하는 wchar_t 
// 2. "ABC"를 대신하는 L"ABC"
// 그냥 "ABC" ? --> 아스키 코드로 표현하겠다라는 암묵적 의미

// ex
wchar_str str[] = L"ABC";