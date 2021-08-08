#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"

CObjectManager* CObjectManager::m_pInst = nullptr;

CObjectManager::CObjectManager() :
	pPlayer(nullptr),m_Monster{},LevelUpExp{},m_DropItem{}
{
}

CObjectManager::~CObjectManager()
{
	if (pPlayer) delete pPlayer;
	for (int i = 0; i < 3; i++)
	{
		if (m_Monster[i]) delete m_Monster[i];
	}
	for (int i = 0; i < 6; i++)
	{
		if (m_DropItem[i]) delete m_DropItem[i];
	}
}

CItem* CObjectManager::CreateItem(const char* Name, Item_Type Type, 
	int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name,Type,Option,Price,Sell, Desc);
	return nullptr;
}

CMonster* CObjectManager::CloneMonster(const char *name)
{
	CMonster* pMonster = nullptr;
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(m_Monster[i]->GetName(), name) == 0)
		{
			pMonster = m_Monster[i];
			break;
		}
	}
	if (pMonster == nullptr) return nullptr;
	return pMonster->Clone();
}

bool CObjectManager::Init()
{
	// 플레이어
	pPlayer = new CPlayer;
	if (!pPlayer->Init()) return false;

	// 몬스터
	m_Monster[0] = new CMonster;
	if (!m_Monster[0]->Init("고블린",200,50,500,500,300,200,1)) return false;
	m_Monster[1] = new CMonster;
	if (!m_Monster[1]->Init("오크",500,300,2000,2000,1000,800,3)) return false;
	m_Monster[2] = new CMonster;
	if (!m_Monster[2]->Init("드래곤",2000,1500,5000,5000,2500,2000,8)) return false;

	// 레벨
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		LevelUpExp[i] = 1000 + 1500 * i;
	}

	// 아이템 
	m_DropItem[0] = CreateItem("목검(레어)", IT_Weapon, 80, 1000, 500, "나무로 만든 검");
	m_DropItem[1] = CreateItem("천갑옷(레어)", IT_Armor, 45, 1000, 500, "약한 방어구");

	m_DropItem[2] = CreateItem("BF.대검(레어)", IT_Weapon, 400, 8000, 4000, "조금 센 검");
	m_DropItem[3] = CreateItem("거인의 벨트(레어)", IT_Armor, 200, 8000, 4000, "조금 단단한 벨트");

	m_DropItem[4] = CreateItem("무한의대검(레어)", IT_Weapon, 900, 40000, 20000, "치명타 맞고 죽어라.");
	m_DropItem[5] = CreateItem("가시갑옷(레어)", IT_Armor, 500, 40000, 20000, "구른다!!");

	return true;
}
