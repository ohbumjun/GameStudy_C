#include "Map.h"
#include "Player.h"
#include "Monster.h"
#include "Inventory.h"
#include "ObjectManager.h"
#include "Item.h"

using namespace std;

CMap::CMap() : m_Type(Map_Type::MT_End)
{
}

CMap::~CMap()
{
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
        cout << "---- Player ----" << endl;
        pPlayer->Output();
        cout << endl;
        cout << "---- Montser ----" << endl;
        pMonster->Output();
        cout << endl;
        switch (Menu())
        {
        case Battle_Menu::Attack:
        {
            switch (Battle(pPlayer,pMonster))
            {
            case Battle_Result::Player_Death:
                pPlayer->Death();
                cout << "�÷��̾� ���" << endl;
                system("pause");
                break;
            case Battle_Result::Monster_Death:
            {
                // ��� �� ����ġ
                cout << "���� ���" << endl;
                pPlayer->AddGold(pMonster->GetGold());
                pPlayer->AddExp(pMonster->GetExp());

                // ������ ����
                if (!CInventory::GetInst()->IsFull())
                {
                    float Percent = (rand() % 10001) / 100.f;
                    if (Percent < 10.f)
                    {
                        CItem* Item = nullptr;
                        int i = rand();
                        if (i % 2 == 0) Item = CObjectManager::GetInst()->GetDropItem(i*(int)m_Type);
                        else Item = CObjectManager::GetInst()->GetDropItem(i * (int)m_Type + 1);
                        CInventory::GetInst()->AddItem(Item->Clone());
                        cout << Item->GetName() << " �� ȹ���߽��ϴ�" << endl;
                    }
                }
                delete pMonster;
                pMonster = SpawnMonster();
                system("pause");
            }
                break;
            }
        }
            break;
        case Battle_Menu::Back:
            return;
        }
    }
}

Battle_Menu CMap::Menu()
{

    cout << "1. ����" << endl;
    cout << "2. ����" << endl;
    cout << "�޴��� �����ϼ��� :";
    int _Menu;
    cin >> _Menu;
    if (_Menu <= 0 || _Menu > 2) return Battle_Menu::None;
    return (Battle_Menu)_Menu;
}

Battle_Result CMap::Battle(CPlayer* player, CMonster* monster)
{
    // �÷��̾� ���� 
    int Attack = player->GetAttack();
    if (player->GetEquipment(Equip_Weapon))
        Attack += player->GetEquipment(Equip_Weapon)->GetOption();
    int Damage = Attack - monster->GetArmor();
    Damage = Damage < 1 ? 1 : Damage;
    if (monster->Damage(Damage)) return Battle_Result::Monster_Death;

    // ���� ���� 
    int Armor = player->GetArmor();
    if (player->GetEquipment(Equip_Armor))
        Armor += player->GetEquipment(Equip_Weapon)->GetOption();
    Damage = monster->GetAttack() - Armor;
    Damage = Damage < 1 ? 1 : Damage;
    if (player->Damage(Damage)) return Battle_Result::Player_Death;
    return Battle_Result::None;
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
}
