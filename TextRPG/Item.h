#pragma once
#include"Game.h"
class CItem
{
public :
	CItem();
	CItem(CItem& Item);
	~CItem();
private :
	char m_Name[52];
	Item_Type m_Type;
	int m_Option;
	int m_Price;
	int m_Sell;
	char m_Desc[512];
public :
	Item_Type GetItemType()
	{
		return m_Type;
	}
	int GetOption() const
	{
		return m_Option;
	}
	int GetPrice() const
	{
		return m_Price;
	}

public :
	CItem* CloneItem();
	void Output();
	bool Init(const char* Name, Item_Type Type, int Option, 
		int Price, int Sell, const char* Desc);
};

