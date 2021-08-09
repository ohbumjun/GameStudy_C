#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "Inventory.h"

using namespace std;

CMap::CMap()
{
};

CMap::~CMap()
{
}

Battle_Menu CMap::Menu()
{
    cout << "1. 공격" << endl;
    cout << "2. ㅌㅌ" << endl;
    int _Menu;
    cin >> _Menu;
    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::Back) return Battle_Menu::None;
    return (Battle_Menu)_Menu;
}

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
    int Attack = pPlayer->GetAttack();
    if (pPlayer->GetEqiupItem(Equip_Weapon))
        Attack += pPlayer->GetEqiupItem(Equip_Weapon)->GetOption();
    int Damage = Attack - pMonster->GetArmor();
    Damage = Damage < 1 ? 1 : Damage;
    if (pMonster->Damage(Damage)) return Battle_Result::Monster_Death;

    int Armor = pPlayer->GetArmor();
    if (pPlayer->GetEqiupItem(Equip_Armor))
        Armor += pPlayer->GetEqiupItem(Equip_Armor)->GetOption();

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
        cout << "==== 몬스터 ====" << endl;
        pMonster->Output();
        cout << endl;
        cout << "==== 플레이어 ====" << endl;
        pPlayer->Output();
        cout << endl;
        switch (Menu())
        {
        case Battle_Menu::Attack:
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Player_Death:
                pPlayer->Death();
                break;
            case Battle_Result::Monster_Death:
                // 경험치, 골드
                pPlayer->AddGold(pMonster->GetGold());
                pPlayer->AddExp(pMonster->GetExp());
                // drop item
                if (!CInventory::GetInst()->IsFull())
                {
                    int Percent = (rand() % 10001) / 100;
                    if (Percent < 10.1)
                    {
                        int Index = -1;
                        if (rand() % 2 == 0)
                        {
                            Index = (int)m_Type * 2;
                        }
                        else
                        {
                            Index = (int)m_Type * 2 + 1;
                        }
                        CItem* DropItem = CObjectManager::GetInst()->GetDropItem(Index);
                        CInventory::GetInst()->AddItem(DropItem->Clone());
                        cout << DropItem->GetName() << " 아이템 획득" << endl;
                        system("pause");
                    }
                }
                // 다시 몬스터 spawn
                delete pMonster;
                pMonster = SpawnMonster();
                break;
            }
            break;
        case Battle_Menu::Back:
            delete pMonster;
            return;
        }
    }
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
}
