#pragma once
#include"Game.h"

class CItem
{
public :
	CItem();
	~CItem();
private :
	char m_Name[100];
	Item_Type m_Type;
	int m_Option;
	int m_Price;
	int m_Sell;
	char m_Desc[512];
public :
	const char* GetDesc() { return m_Desc; }
	int GetPrice() { return m_Price; }
	int GetOption() { return m_Option; }
	int GetSell() { return m_Sell; }
	Item_Type GetItemType(){ return m_Type; }
	const char* GetName() { return m_Name; }
public :
	bool Init(const char* Name, Item_Type Type, int Option,
		int Price, int Sell, const char* Desc);
	void Output();
	CItem* Clone();
};

