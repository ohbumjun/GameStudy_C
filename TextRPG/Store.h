#pragma once
#include"Game.h"

class CStore
{
public :
	CStore();
	~CStore();
private:
	class CItem* m_Item[IT_End];
	Store_Type m_Type;
public :
	class CItem* CreateItem(const char* Name, Item_Type Item, int Option,
		int Price, int Sell, const char* Desc );
	bool Init(const Store_Type& Type);
	void SetWeaponList();
	void SetArmorList();
	void Run();
	int Menu(class CPlayer* pPlayer);
};