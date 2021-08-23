#include "Inventory.h"
#include "Player.h"
#include "Item.h"
#include "ObjectManager.h"

using namespace std;

CInventory* CInventory::m_pInst = nullptr;
CInventory::CInventory() : 
    m_Size(0) ,
    m_Item{}
{
}
CInventory::~CInventory()
{
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Item[i]) delete m_Item[i];
    }
}
void CInventory::Run()
{
    CPlayer* player = CObjectManager::GetInst()->GetPlayer();

    while (true)
    {
        int Select = Menu(player);
        if (Select == -1) continue;
        if (Select == 0) return;
        int Index = Select - 1;
        CItem* Item = player->Equip(m_Item[Index]->Clone());
        if (Item)
        {
            m_Item[Index] = Item;
        }
        else
        {
            for (int i = Index; i < m_Size - 1; i++)
                m_Item[i] = m_Item[i + 1];
            m_Item[m_Size - 1] = nullptr;
            --m_Size;
        }
    }
}

bool CInventory::Init()
{
    return true;
}

int CInventory::Menu(CPlayer* Player)
{
    system("cls");
    cout << "장착 무기 : ";
    if (Player->GetEquipment(Equip_Weapon))
        cout << Player->GetEquipment(Equip_Weapon)->GetName() << endl;
    cout << endl;
    cout << "장착 방어구 : ";
    if (Player->GetEquipment(Equip_Armor))
        cout << Player->GetEquipment(Equip_Armor)->GetName() << endl;
    cout << endl;

    cout << "---- 인벤토리 ----" << endl;
    for (int i = 0; i < m_Size; i++)
    {
        cout << i+ 1 << ". ";
        m_Item[i]->Output();
        cout << endl;
    }
    cout << "0. 뒤로가기" << endl;
    cout << "장착할 아이템을 선택하세요 :";
    int _Menu;
    cin >> _Menu;
    if (_Menu < 0 || _Menu > m_Size) return -1;
    return _Menu;

    return 0 ;
}
