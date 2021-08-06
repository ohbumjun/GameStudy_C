#pragma once
#include"Game.h"
class CItem
{
public :
	CItem();
	~CItem();
public :
	bool Init(const char* Name, Item_Type Type, int Option, 
		int Price, int Sell, const char* Desc);
};

