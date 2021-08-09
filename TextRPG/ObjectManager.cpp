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
	// �÷��̾�
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;
	
	// ����
	m_Monster[0] = new CMonster;
	if (!m_Monster[0]->Init("���",300,100,500,500,400,250,1)) return false;
	m_Monster[1] = new CMonster;
	if (!m_Monster[1]->Init("��ũ",1000,400,2000,1000,1500,1000,4)) return false;
	m_Monster[2] = new CMonster;
	if (!m_Monster[2]->Init("�巡��",3000,1500,10000,10000,5000,3000,10)) return false;

	// Level
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		m_LevelUpExp[i] = 1000 + 2000 * i;
	}


	// drop Item
	m_DropItem[0] = new CItem;
	if (!m_DropItem[0]->Init("������", IT_Weapon, 300, 1000, 500, "�����θ����")) return false;
	m_DropItem[1] = new CItem;
	if (!m_DropItem[1]->Init("��������", IT_Armor, 300, 1000, 500, "�����θ������")) return false;
	m_DropItem[2] = new CItem;
	if (!m_DropItem[2]->Init("����", IT_Weapon, 300, 1000, 500, "���θ����")) return false;
	m_DropItem[3] = new CItem;
	if (!m_DropItem[3]->Init("������", IT_Weapon, 300, 1000, 500, "���θ������")) return false;
	m_DropItem[4] = new CItem;
	if (!m_DropItem[4]->Init("���", IT_Weapon, 300, 1000, 500, "��θ����")) return false;
	m_DropItem[5] = new CItem;
	if (!m_DropItem[5]->Init("�����", IT_Weapon, 300, 1000, 500, "�踸�����")) return false;

	return true;
}

CItem* CObjectManager::CreateItem(const char* Name,Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name, Type, Option, Price, Sell, Desc);
	return Item;
}
