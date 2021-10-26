#include "ObjectManager.h"
#include "Monster.h"
#include "Player.h"
#include "Item.h"

using namespace std;

CObjectManager* CObjectManager::m_pInst = nullptr;
CObjectManager::CObjectManager() :
	m_Player(nullptr),
	m_Monster{},
	m_DropItems{}
{

}
CObjectManager::~CObjectManager()
{
	if (m_Player) delete m_Player;
	for (int i = 0; i < 3; i++)
	{
		if (m_Monster[i]) delete m_Monster[i];
	}
	for (int i = 0; i < 6; i++)
	{
		if (m_DropItems[i]) delete m_DropItems[i];
	}
}

bool CObjectManager::Init()
{
	// 플레이어
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;

	cout << "player set complete" << endl;
	
	// 몬스터
	m_Monster[0] = new CMonster;
	if (!m_Monster[0]->Init("고블린", 400, 300, 2000, 500, 300, 200, 1))return false;
	m_Monster[1] = new CMonster;
	if (!m_Monster[1]->Init("오크", 800, 600, 6000, 700, 300, 200, 2)) return false;
	m_Monster[2] = new CMonster;
	if (!m_Monster[2]->Init("드래곤", 2000, 1000, 15000, 600, 400, 200, 3)) return false;

	// 경험치 테이블
	m_LevelUpExp[0] = 5000;
	m_LevelUpExp[1] = 12000;
	m_LevelUpExp[2] = 18000;
	m_LevelUpExp[3] = 25000;
	m_LevelUpExp[4] = 40000;
	m_LevelUpExp[5] = 60000;
	m_LevelUpExp[6] = 80000;
	m_LevelUpExp[7] = 100000;
	m_LevelUpExp[8] = 120000;
	m_LevelUpExp[9] = 140000;

	// 아이템
	m_DropItems[0] = new CItem;
	m_DropItems[0]->Init("나무칼", IT_Weapon, 2000, 200, 200, "나무로만든칼");

	m_DropItems[1] = new CItem;
	m_DropItems[1]->Init("나무방패", IT_Armor, 2000, 200, 200, "나무로만든방패");

	m_DropItems[2] = new CItem;
	m_DropItems[2]->Init("철칼", IT_Weapon, 4000, 400, 400, "철로만든칼");

	m_DropItems[3] = new CItem;
	m_DropItems[3]->Init("철방패", IT_Armor, 4000, 400, 400, "철로만든방패");

	m_DropItems[4] = new CItem;
	m_DropItems[4]->Init("금칼", IT_Weapon, 2000, 600, 600, "금으로만든칼");

	m_DropItems[5] = new CItem;
	m_DropItems[5]->Init("금방패", IT_Armor, 6000, 800, 800, "금으로만든방패");
	return true;
}

CMonster* CObjectManager::CloneMonster(const char* Name)
{
	CMonster* p = nullptr;
	for (int i = 0; i < MT_End; i++)
	{
		if (strcmp(Name, m_Monster[i]->GetName()) == 0)
		{
			p = m_Monster[i]->Clone();
			break;
		}
	}
	return p;
}

CItem* CObjectManager::CloneItem(CItem* Item)
{
	return nullptr;
}

CItem* CObjectManager::CreateItem(const char* Name, Item_Type Type, 
	int Price, int Option, int Sell, const char* Desc)
{
	CItem* Item = nullptr;
	Item->Init(Name, Type, Price, Option, Sell, Desc);
	return Item;
}
