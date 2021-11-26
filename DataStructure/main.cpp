#include<iostream>
#include<tchar.h>
#include<Windows.h>

#define DIR_LEN MAX_PATH + 1

/*
typedef char CHAR
typedef wchar_t WCHAR

typedef CHAR* LPSTR;
typedef CONST CHAR* LPCSTR;

typedef WCHAR* LPWSTR;
typedef CONST WCHAR* LPCWSTR;
*/

#ifdef UNICODE 
typedef WCHAR TCHAR;
typedef LPWSTR LPSTR;
typedef LPCWSTR LPCTSTR;
#else 
typedef CHAR TCHAR;
typedef LPSTR LTPSTR;
typedef LPCSTR LPCSTR;
#endif

#ifdef _UNICODE 
#define __T(x) L ## x
#else 
#define __T(x) x 
#endif 

#define _T(x) __T(x)
#define _TEXT(x) __T(x)

// #define _UNICODE
// #define UNICODE


int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si); // 구조체 변수의 크기 
	si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	si.dwX = 100;
	si.dwY = 200;
	si.dwXSize = 300;
	si.dwYSize = 300;
	si.lpTitle = _T("I am a boy");
	return 0;
}