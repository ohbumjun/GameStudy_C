#pragma once
#include"Game.h"
class CMap
{
public :
	CMap();
	~CMap();
private :
	Map_Type m_Type;
public :
	Battle_Menu Menu();
	Battle_Result Battle(class CPlayer* pPlayer, class CMonster* pMonster);
	bool Init(Map_Type Type);
	void Run();
	class CMonster* SpawnMonster();
};

