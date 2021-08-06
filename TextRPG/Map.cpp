#include "Map.h"
#include "ObjectManager.h"

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
    cout << "1. ����" << endl;
    cout << "2. ����" << endl;
    cout << "3. �ڷΰ���" << endl;
    cout << "�޴��� �����ϼ��� : ";

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
    return Battle_Result();
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
