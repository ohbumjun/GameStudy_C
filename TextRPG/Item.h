#pragma once
#include"Game.h"

class CItem
{
public :
	CItem();
	CItem(const CItem& Item);
	~CItem();
private:
	char m_Name[52];
	Item_Type m_Type;
	int m_Option;
	int m_Price;
	int m_Sell;
	char m_Desc[512];
public :
	const char* GetName() const { return m_Name; }
	Item_Type GetItemType() const { return m_Type; }
	int GetPrice() const { return m_Price; }
	int GetOption() const { return m_Option; }
	int GetSell() const { return m_Sell; }
	const char* GetDesc() const { return m_Desc; }

public :
	CItem* Clone();
	bool Init(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc);
	void Output();
};

