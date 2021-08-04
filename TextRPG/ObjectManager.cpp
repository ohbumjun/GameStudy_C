#include "ObjectManager.h"
#include"Player.h"
#include"Monster.h"
#include"Item.h"

CObjectManager* CObjectManager::m_pInst = nullptr;
CObjectManager::CObjectManager() : m_DropItem{}
{
}

CObjectManager::~CObjectManager()
{
}

bool CObjectManager::Init()
{
	// 플레이어
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;

	// 몬스터 
	m_Monster[0] = new CMonster;
	m_Monster[0]->Init("고블린", 100, 100, 500, 300, 1, 500, 500);
	m_Monster[1] = new CMonster;
	m_Monster[1]->Init("오크", 500, 500, 2000, 500, 3, 1500, 2000);
	m_Monster[2] = new CMonster;
	!m_Monster[2]->Init("드래곤", 1500, 2000, 15000, 5000, 3, 4000, 5000);

	// 경험치 세팅
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		m_LevelUpExp[i] = 1000 * 1500 * i;
	}

	// DropItem 세팅
	m_DropItem[0] = new CItem;
	m_DropItem[0]->Init("나무검", IT_Weapon, 300, 500, 200, "나무검이다");
	m_DropItem[1] = new CItem;
	m_DropItem[1]->Init("나무방패", IT_Armor, 300, 500, 200, "나무방패다");
	m_DropItem[2] = new CItem;
	m_DropItem[2]->Init("철검", IT_Weapon, 600, 2000, 1000, "철검이다");
	m_DropItem[3] = new CItem;
	m_DropItem[3]->Init("철방패", IT_Armor, 600, 2000, 1000, "철방패다");
	m_DropItem[4] = new CItem;
	m_DropItem[4]->Init("황금검", IT_Weapon, 2000, 5000, 2000, "황금검이다");
	m_DropItem[5] = new CItem;
	m_DropItem[5]->Init("황금방패", IT_Armor, 2000, 5000, 2000, "황금방패다");
	
	return true;
}

void CObjectManager::Run()
{
}

CMonster* CObjectManager::CloneMonster(const char *Name)
{
	CMonster* pFindMonster = nullptr;
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(m_Monster[i]->GetName() , Name) == 0)
		{
			pFindMonster = m_Monster[i];
			break;
		}
	}

	if (pFindMonster == nullptr)
		return nullptr;
	return pFindMonster->Clone();
}

CItem* CObjectManager::CloneItem(int Index)
{
	return m_DropItem[Index]->Clone();
}
