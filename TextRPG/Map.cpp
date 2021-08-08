#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"

using namespace std;

CMap::CMap() : m_Type(MT_Easy)
{
}

CMap::~CMap()
{
}

Battle_Menu CMap::Menu()
{
    cout << "1. 공격" << endl;
    cout << "2. ㅌㅌ" << endl;
    cout << "메뉴를 선택하세요 : ";

    int _Menu;
    cin >> _Menu;

    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::Back)
        return Battle_Menu::None;

    return (Battle_Menu)_Menu;
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

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
    int Attack = pPlayer->GetAttack();
    if (pPlayer->GetEquipment(Equip_Weapon))
        Attack += pPlayer->GetEquipment(Equip_Weapon)->GetOption();

    int Damage = Attack - pMonster->GetArmor();
    Damage = Damage < 1 ? 1 : Damage;

    if (pMonster->Damage(Damage))
        return Battle_Result::Monster_Death;

    int Armor = pPlayer->GetArmor();
    if (pPlayer->GetEquipment(Equip_Armor))
        Armor += pPlayer->GetEquipment(Equip_Armor)->GetOption();

    Damage = pMonster->GetAttack() - Armor;
    Damage = Damage < 1 ? 1 : Damage;

    if (pPlayer->Damage(Damage))
        return Battle_Result::Player_Death;

    return Battle_Result::None;
}

void CMap::Run()
{
    system("cls");
    CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
    CMonster* pMonster = SpawnMonster();
    cout << "==== 플레이어 ====" << endl;
    pPlayer->Output();
    cout << "==== 몬스터 ====" << endl;
    pMonster->Output();
    cout << endl;
    while (true)
    {
        switch (Menu())
        {
        case Battle_Menu::Attack:
        {
            cout << "attack" << endl;
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Player_Death:
                pPlayer->Death();
                break;
            case Battle_Result::Monster_Death:
                // 경험치, 골드
                pPlayer->AddGold(pMonster->GetGold());
                pPlayer->AddExp(pMonster->GetExp());
                // 드롭아이템
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

bool CMap::Init()
{
    return true;
}
