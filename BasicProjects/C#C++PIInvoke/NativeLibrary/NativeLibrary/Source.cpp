#include <iostream>
#include <comdef.h>

extern "C" __declspec(dllexport) void HelloWorld();
extern "C" __declspec(dllexport) int Add(int, int);
extern "C" __declspec(dllexport) bool LengthGreaterThan5(const char*);
extern "C" __declspec(dllexport) BSTR GetName();

struct Shoe
{
	int id;
	BSTR color;
	void buy()
	{
		std::wcout << "color : " << color << std::endl;
	}
};

extern "C" __declspec(dllexport) void BuyShoe(Shoe shoe)
{
	shoe.buy();
}

extern "C" __declspec(dllexport) Shoe CreateShoe(double shoeSize)
{
	Shoe newShoe = Shoe();

	newShoe.id = 2;
	newShoe.color = SysAllocString(L"blue");

	return newShoe;
}

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
	// string �����Ͱ� �ش� ���� ����� �޸� �������� �ʵ��� �ϱ� 
	// Allocate �ϴµ�, �� ���⼭ clean ���� ? -> c# ���� [return: MarshalAs(UnmanagedType.BStr)] �� clean ����
	return SysAllocString(L"BJ");
}