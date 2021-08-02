#pragma once
#include"Game.h"

class CItem
{
	CItem();
	~CItem();
private :
	char m_Name[52];
	Item_Type m_Type;
	int m_Option;
	int m_Price;
	int m_Sell;
public :
	int GetPrice() { return m_Price; }
	int GetOption() { return m_Option; }
	int GetSell() { return m_Sell; }
	Item_Type GetItemType(){ return m_Type; }
	const char* GetName() { return m_Name; }
};

