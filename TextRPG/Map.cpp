#include "Map.h"
#include"Player.h"
#include"Monster.h"
#include"ObjectManager.h"
#include"Inventory.h"
#include"Item.h"

using namespace std;

CMap::CMap() :
	m_Type(Map_Type::MT_Easy)
{
}

CMap::~CMap()
{
}

CMonster* CMap::SpawnMonster()
{
	switch (m_Type)
	{
	case MT_Easy:
		return CObjectManager::GetInst()->CloneMonster("고블린");
	case MT_Normal:
		return CObjectManager::GetInst()->CloneMonster("오크");
	case MT_Hard:
		return CObjectManager::GetInst()->CloneMonster("드래곤");
	}
	return nullptr;
}

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
	int Attack = pPlayer->GetAttack();
	if (pPlayer->GetEquipItem(Equip_Weapon))
		Attack += pPlayer->GetEquipItem(Equip_Weapon)->GetOption();
	int Damage = Attack - pMonster->GetArmor();
	Damage = Damage < 1 ? 1 : Damage;
	if (pMonster->Damage(Damage)) return Battle_Result::Monster_Death;

	int Armor = pPlayer->GetArmor();
	if(pPlayer->GetEquipItem(Equip_Armor))
		Armor += pPlayer->GetEquipItem(Equip_Armor)->GetOption();
	Damage = pMonster->GetAttack() - Armor;
	Damage = Damage < 1 ? 1 : Damage;
	if (pPlayer->Damage(Damage)) return Battle_Result::Player_Death;

	return Battle_Result::None;
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
		system("cls");
		cout << "==== 몬스터 ==== " << endl;
		pMonster->Output();
		cout << endl;

		cout << "==== 플레이어 ====" << endl;
		pPlayer->Output();
		cout << endl;

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
					float Percent = rand() % 10001 / 100.f;
					// 10% 확률로 drop Item
					if (Percent < 10.1f)
					{
						if (rand() % 2 == 0) Index = (int)m_Type * 2;
						else Index = (int)m_Type * 2 + 1;
						CItem* DropItem = CObjectManager::GetInst()->CloneItem(Index);
						CInventory::GetInst()->AddItem(DropItem);
						cout << DropItem->GetName() << " 아이템 획득" << endl;
						system("pause");
					}
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

	cout << "1. 공격" << endl;
	cout << "2. ㅌㅌ" << endl;
	cout << "메뉴를 선택하세요";
	int _Menu;
	cin >> _Menu;

	if ((int)Battle_Menu::None >= _Menu || _Menu > (int)Battle_Menu::Back) return Battle_Menu::None;
	return (Battle_Menu)_Menu ;
}
