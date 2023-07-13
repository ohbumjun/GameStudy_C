#include <iostream>
#include <comdef.h>

extern "C" __declspec(dllexport) void HelloWorld();
extern "C" __declspec(dllexport) int Add(int, int);
extern "C" __declspec(dllexport) bool LengthGreaterThan5(const char*);
extern "C" __declspec(dllexport) BSTR GetName();

void HelloWorld()
{
	std::cout << "Hello world" << std::endl;
};

int Add(int a, int b)
{
	return a + b;
}

bool LengthGreaterThan5(const char* value)
{
	return strlen(value) > 5;
}

BSTR GetName()
{
	// string 데이터거 해당 영역 벗어나도 메모리 해제되지 않도록 하기 
	// Allocate 하는데, 왜 여기서 clean 안함 ? -> c# 상의 [return: MarshalAs(UnmanagedType.BStr)] 가 clean 해줌
	return SysAllocString(L"BJ");
}