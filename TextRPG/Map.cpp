#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "Inventory.h"

using namespace std;

CMap::CMap() : m_Type(MT_Easy)
{
}

CMap::~CMap()
{
}

Battle_Menu CMap::Menu()
{
    
    cout << "1. ����" << endl;
    cout << "2. ����" << endl;
    cout << "�޴��� �����ϼ��� : ";

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
        return CObjectManager::GetInst()->CloneMonster("���");
    case MT_Normal:
        return CObjectManager::GetInst()->CloneMonster("��ũ");
    case MT_Hard:
        return CObjectManager::GetInst()->CloneMonster("�巡��");
    }

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
        system("cls");
        cout << "==== �÷��̾� ====" << endl;
        pPlayer->Output();
        cout << "==== ���� ====" << endl;
        pMonster->Output();
        cout << endl;
        switch (Menu())
        {
        case Battle_Menu::Attack:
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Monster_Death:
                // ����ġ, ���
                pPlayer->AddGold(pMonster->GetGold());
                pPlayer->AddExp(pMonster->GetExp());
                // ��Ӿ�����
                if (!CInventory::GetInst()->IsFull())
                {
                    float Percent = (rand() % 10001) / 100;
                    if (Percent < 10.1f)
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
                        // �κ��丮 â�� �߰��ϱ� 
                        CInventory::GetInst()->AddItem(CObjectManager::GetInst()->GetDropItem(Index));
                    }
                }
                // ���ο� ���� spawn
                delete pMonster;
                pMonster = SpawnMonster();
                system("pause");
                break;
            case Battle_Result::Player_Death:
                pPlayer->Death();
                break;
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
