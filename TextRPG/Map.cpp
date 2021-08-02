#include "Map.h"
#include"Player.h"
#include"Monster.h"
#include"ObjectManager.h"
#include"Inventory.h"

using namespace std;

CMap::CMap()
{
}

CMap::~CMap()
{
}

CMonster* CMap::SpawnMonster()
{
	return nullptr;
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
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	CMonster* pMonster = SpawnMonster();
	while (true)
	{
		switch (Menu())
		{
		case Battle_Menu::Attack:
		{
			switch (Battle(pPlayer, pMonster))
			{
			case Battle_Result::Monster_Death:
			{
				pPlayer->AddGold(pMonster->GetGold());
				pPlayer->AddExp(pMonster->GetExp());

				int Index = -1;
				// 인벤토리가 가득차지 않았다면 : add Item
				if (!CInventory::GetInst()->IsFull())
				{
					int Percent = rand() % 10001 / 100;
					// 10% 확률로 drop Item
					if (Percent < 10.1f)
					{
						if (rand() % 2 == 0) Index = (int)m_Type * 2;
						else Index = (int)m_Type * 2 + 1;
					}
					CItem* DropItem = CObjectManager::GetInst()->GetDropItem(Index);
					CInventory::GetInst()->AddItem(DropItem);
				}
				delete pMonster;
				pMonster = SpawnMonster();
			}
				break;
			case Battle_Result::Player_Death:
				pPlayer->Death();
				break;
			}
		}
			break;
		case Battle_Menu::Back:
			delete pMonster;
			return;
		}
	}
}

Battle_Menu CMap::Menu()
{
	system("cls");

	cout << "1. 공격" << endl;
	cout << "2. ㅌㅌ" << endl;
	cout << "메뉴를 선택하세요";
	int _Menu;
	cin >> _Menu;

	if ((int)Battle_Menu::None > _Menu || _Menu > (int)Battle_Menu::Back) return Battle_Menu::None;
	return (Battle_Menu)_Menu ;
}
