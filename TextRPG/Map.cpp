#include "Map.h"

CMap::CMap()
{
}

CMap::~CMap()
{
}

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
	return Battle_Result();
}

bool CMap::Init(Map_Type Type)
{
	m_Type = Type;
	return true;
}

void CMap::Run()
{
}

Battle_Menu CMap::Menu()
{
	system("cls");
	return Battle_Menu();
}
