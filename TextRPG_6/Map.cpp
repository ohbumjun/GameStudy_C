#include "Map.h"
#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"

using namespace std;

void CMap::Run()
{
    CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
    while (true)
    {
        switch (Menu())
        {
        case Battle_Menu::Attack:
        {
            switch (Battle())
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
        case Battle_Menu::Back:
            break;
        }
    }
}

Battle_Menu CMap::Menu()
{
    cout << "1. 공격" << endl;
    cout << "2. ㅌㅌ" << endl;
    cout << "메뉴를 선택하세요 :";
    int _Menu;
    cin >> _Menu;
    if (_Menu <= 0 || _Menu > 2) return Battle_Menu::None;
    return (Battle_Menu)_Menu;
}

Battle_Result CMap::Battle(CPlayer* player, CMonster* monster)
{
    return Battle_Result();
}

CMonster* CMap::SpawnMonster()
{
    return nullptr;
}
