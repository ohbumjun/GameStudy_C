#pragma once
#include "Game.h"

class CMap
{
public :
	CMap()
	{

	}
	~CMap()
	{

	}
private :
	Map_Type m_Type;
public :
	bool Init(Map_Type Type);
	void Run();
	Battle_Menu Menu();
	Battle_Result Battle(class CPlayer* player, class CMonster* monster);
	class CMonster* SpawnMonster();
};

