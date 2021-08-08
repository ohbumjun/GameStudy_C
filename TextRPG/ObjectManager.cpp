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
	// �÷��̾�
	pPlayer = new CPlayer;
	if (!pPlayer->Init()) return false;

	// ����
	m_Monster[0] = new CMonster;
	if (!m_Monster[0]->Init("���",200,50,500,500,300,200,1)) return false;
	m_Monster[1] = new CMonster;
	if (!m_Monster[1]->Init("��ũ",500,300,2000,2000,1000,800,3)) return false;
	m_Monster[2] = new CMonster;
	if (!m_Monster[2]->Init("�巡��",2000,1500,5000,5000,2500,2000,8)) return false;

	// ����
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		LevelUpExp[i] = 1000 + 1500 * i;
	}

	// ������ 
	m_DropItem[0] = CreateItem("���(����)", IT_Weapon, 80, 1000, 500, "������ ���� ��");
	m_DropItem[1] = CreateItem("õ����(����)", IT_Armor, 45, 1000, 500, "���� ��");

	m_DropItem[2] = CreateItem("BF.���(����)", IT_Weapon, 400, 8000, 4000, "���� �� ��");
	m_DropItem[3] = CreateItem("������ ��Ʈ(����)", IT_Armor, 200, 8000, 4000, "���� �ܴ��� ��Ʈ");

	m_DropItem[4] = CreateItem("�����Ǵ��(����)", IT_Weapon, 900, 40000, 20000, "ġ��Ÿ �°� �׾��.");
	m_DropItem[5] = CreateItem("���ð���(����)", IT_Armor, 500, 40000, 20000, "������!!");

	return true;
}
