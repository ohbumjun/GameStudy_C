#pragma once
#include"Game.h"

class CItem
{
public:
	CItem();
	CItem(const CItem* Item);
	~CItem();
private :
	char m_Name[32];
	Item_Type m_Type;
	int m_Price;
	int m_Option;
	int m_Sell;
	char m_Desc[52];
public :
	int GetOption()const { return m_Option; }
	int GetPrice()const { return m_Price; }
	const char* GetName() const { return m_Name; }
	const char* GetDesc() const { return m_Desc; }
	Item_Type GetItemType() const { return m_Type; }
public :
	bool Init(const char*Name, Item_Type Type,
		int Price, int Option, int Sell, 
		const char* Desc);
	void Output();
	CItem* Clone();
};

