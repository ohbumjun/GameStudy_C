#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "Inventory.h"

using namespace std;

CMap::CMap()
{
}

Battle_Menu CMap::Menu()
{
    system("cls");
    cout << "1. 공격" << endl;
    cout << "2. ㅌㅌ" << endl;
    int _Menu;
    cin >> _Menu;
    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::Back) return Battle_Menu::None;
    return (Battle_Menu)_Menu;
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
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Player_Death:
                pPlayer->Death();
                break;
            case Battle_Result::Monster_Death:
                // 경험치, 골드
                pPlayer->AddGold(pMonster->GetGold());

                // drop item

                // 다시 몬스터 spawn
                delete pMonster;
                pMonster = SpawnMonster();
                break;
            default:
                break;
            }
        }
            break;
        case Battle_Menu::Back:
            delete pMonster;
            break;
        }
    }
}

CMonster* CMap::SpawnMonster()
{
    return nullptr;
}
`