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
	// �÷��̾�
	m_Player = new CPlayer;
	if (!m_Player->Init()) return false;

	// ���� 
	m_Monster[0] = new CMonster;
	m_Monster[0]->Init("���", 100, 100, 500, 300, 1, 500, 500);
	m_Monster[1] = new CMonster;
	m_Monster[1]->Init("��ũ", 500, 500, 2000, 500, 3, 1500, 2000);
	m_Monster[2] = new CMonster;
	!m_Monster[2]->Init("�巡��", 1500, 2000, 15000, 5000, 3, 4000, 5000);

	// ����ġ ����
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		m_LevelUpExp[i] = 1000 * 1500 * i;
	}

	// DropItem ����
	m_DropItem[0] = new CItem;
	m_DropItem[0]->Init("������", IT_Weapon, 300, 500, 200, "�������̴�");
	m_DropItem[1] = new CItem;
	m_DropItem[1]->Init("��������", IT_Armor, 300, 500, 200, "�������д�");
	m_DropItem[2] = new CItem;
	m_DropItem[2]->Init("ö��", IT_Weapon, 600, 2000, 1000, "ö���̴�");
	m_DropItem[3] = new CItem;
	m_DropItem[3]->Init("ö����", IT_Armor, 600, 2000, 1000, "ö���д�");
	m_DropItem[4] = new CItem;
	m_DropItem[4]->Init("Ȳ�ݰ�", IT_Weapon, 2000, 5000, 2000, "Ȳ�ݰ��̴�");
	m_DropItem[5] = new CItem;
	m_DropItem[5]->Init("Ȳ�ݹ���", IT_Armor, 2000, 5000, 2000, "Ȳ�ݹ��д�");
	
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
