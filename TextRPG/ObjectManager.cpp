#include "ObjectManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"

using namespace std;

CObjectManager* CObjectManager::m_pInst = nullptr;
CObjectManager::CObjectManager() :
	m_Player(nullptr),
	m_Monster{},
	m_DropItem{},
	m_LevelUpExp{}
{
}

CObjectManager::~CObjectManager()
{
	if (m_Player) delete m_Player;
	for (int i = 0; i < MT_End; i++)
	{
		if (m_Monster[i]) delete m_Monster[i];
	}
	for (int i = 0; i < 6; i++)
	{
		if (m_DropItem[i]) delete m_DropItem[i];
	}
};


CMonster* CObjectManager::CloneMonster(const char* Name)
{
	CMonster* pFoundMonster = nullptr;
	for (int i = 0; i < 3; i++)
	{
		cout << "monster Name : " << m_Monster[i]->GetName() << endl;
		if (strcmp(Name, m_Monster[i]->GetName()) == 0)
		{
			pFoundMonster = m_Monster[i];
			break;
		}
	}
	if (pFoundMonster == nullptr) return nullptr;
	return pFoundMonster->Clone();
}

bool CObjectManager::Init()
{
	// 플레이어
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;
	
	// 몬스터
	m_Monster[0] = new CMonster;
	if (!m_Monster[0]->Init("고블린",300,100,500,500,400,250,1)) return false;
	m_Monster[1] = new CMonster;
	if (!m_Monster[1]->Init("오크",1000,400,2000,1000,1500,1000,4)) return false;
	m_Monster[2] = new CMonster;
	if (!m_Monster[2]->Init("드래곤",3000,1500,10000,10000,5000,3000,10)) return false;

	// Level
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		m_LevelUpExp[i] = 1000 + 2000 * i;
	}


	// drop Item
	m_DropItem[0] = new CItem;
	if (!m_DropItem[0]->Init("나무검", IT_Weapon, 300, 1000, 500, "나무로만든검")) return false;
	m_DropItem[1] = new CItem;
	if (!m_DropItem[1]->Init("나무방패", IT_Armor, 300, 1000, 500, "나무로만든방패")) return false;
	m_DropItem[2] = new CItem;
	if (!m_DropItem[2]->Init("돌검", IT_Weapon, 300, 1000, 500, "돌로만든검")) return false;
	m_DropItem[3] = new CItem;
	if (!m_DropItem[3]->Init("돌방패", IT_Weapon, 300, 1000, 500, "돌로만든방패")) return false;
	m_DropItem[4] = new CItem;
	if (!m_DropItem[4]->Init("쇠검", IT_Weapon, 300, 1000, 500, "쇠로만든검")) return false;
	m_DropItem[5] = new CItem;
	if (!m_DropItem[5]->Init("쇠방패", IT_Weapon, 300, 1000, 500, "쇠만든방패")) return false;

	return true;
}

CItem* CObjectManager::CreateItem(const char* Name,Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name, Type, Option, Price, Sell, Desc);
	return Item;
}
