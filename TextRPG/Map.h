#pragma once
#include"Game.h"
class CMap
{
public :
	CMap();
	~CMap();
private:
	Battle_Menu Menu();
public:
	class CMonster* SpawnMonster();
	Battle_Result Battle(class CPlayer* pPlayer, class CMonster* pMonster);
	void Run();
	bool Init();
};

