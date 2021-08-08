#pragma once
#include"Game.h"
class CStore
{
public :
	CStore();
	~CStore();
private :
	Store_Type m_Type;
	class CItem* m_Item[3];
public :
	class CItem* CreateItem(const char* Name,
		Item_Type Type, int Option,
		int Price, int Sell, const char* Desc);

	bool Init(Store_Type Type);
	void Run();
	int Menu(class CPlayer *pPlayer);
	void SetWeaponList();
	void SetArmorList();
};

