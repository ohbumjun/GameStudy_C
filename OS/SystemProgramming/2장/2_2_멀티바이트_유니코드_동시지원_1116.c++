/*
Window 정의 자료형
typedef char , wchar_t ?

typedef char CHAR
typedef wchar_t WCHAR

typedef CHAR* LPSTR;
typedef CONST CHAR* LPCSTR;

typedef WCHAR* LPWSTR;
typedef CONST WCHAR* LPCWSTR;

*/

// UNICODE 선언에 따라서
// 멀티바이트로 처리될지, 유니코드로 처리될지가
// 결정되는 것이다 
#ifdef UNICODE 
    typedef WCHAR TCHAR;
    typedef LPWSTR LPSTR;
    typedef LPCWSTR LPCTSTR;
#else 
    typedef CHAR TCHAR;
    typedef LPSTR LTPSTR;
    typedef LPCSTR LPCSTR;
#endif

// unicode ?
// TCHAR arr[10] --> WCHAR arr[10] --> wchar_t arr[10];
// X ?
// TCHAR arr[10] --> CHAR arr[10] --> char_t arr[10]


#ifdef _UNICODE 
#define __T(x) L ## x
#else 
#define __T(x) x 
#endif 

#define _T(x) __T(x)
#define _TEXT(x) __T(x)

// unicode ?
_T("HANBIT") --> __T("HANBIT") --> L"HANBIT"
_T("HANBIT") --> __T("HANBIT") --> "HANBIT"

#define UNICODE
#define _UNICODE

int wmain()
{
    TCHA str[] = T("1234567");
    int size = sizeof(str);

    printf("string length : %d", size);
}