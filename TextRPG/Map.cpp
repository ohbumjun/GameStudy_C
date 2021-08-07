#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"

using namespace std;

CMap::CMap()
{
}

CMap::~CMap()
{
}

Battle_Menu CMap::Menu()
{
    system("cls");
    cout << "1. 공격" << endl;
    cout << "2. ㅌㅌ" << endl;
    cout << "3. 뒤로가기" << endl;
    cout << "메뉴를 선택하세요 : ";

    int _Menu;
    cin >> _Menu;

    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::End)
        return Battle_Menu::None;

    return (Battle_Menu)_Menu;
}

CMonster* CMap::SpawnMonster()
{
    return nullptr;
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
    CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
    CMonster* pMonster = SpawnMonster();
    while (true)
    {
        switch (Menu())
        {
        case Battle_Menu::Attack:
        {
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Player_Death:
                break;
            case Battle_Result::Monster_Death:
                break;
            default:
                break;
            }
        }
            break;
        case Battle_Menu::Armor:
            break;
        case Battle_Menu::End:
            break;
        default:
            break;
        }
    }

}

bool CMap::Init()
{
    return true;
}
