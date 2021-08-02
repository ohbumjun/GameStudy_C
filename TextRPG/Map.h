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
	Battle_Result Battle(class CPlayer* pPlayer, class CMonster* pMonster);
public :
	bool Init(Map_Type Type);
	void Run();
	Battle_Menu Menu();
};

