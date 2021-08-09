#include "Inventory.h"
#include "Item.h"
#include "ObjectManager.h"
#include "Player.h"

using namespace std;

CInventory* CInventory::m_pInst = nullptr;

CInventory::CInventory() :
	m_Item{},m_Size(0)
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

bool CInventory::Init()
{
	return true;
}
void CInventory::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(pPlayer);
		if (Select == -1) continue;
		if (Select == 0) return;
		int Index = Select - 1;
		CItem* Item = Equip(m_Item[Index]);
		if (Item)
		{
			m_Item[Index] = Item;
		}
		else
		{
			for (int i = Index; i < m_Size - 1; i++)
			{
				m_Item[i] = m_Item[i + 1];
			}
			m_Item[m_Size] = nullptr;
			--m_Size;
		}
	}
}
int CInventory::Menu(CPlayer* pPlayer)
{
	system("cls");
	cout << "==== 인벤토리 ====" << endl;
	
	for (int i = 0; i < m_Size; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "0. 뒤로가기" << endl;

	cout << "장착 무기 : ";
	CItem* EquipItem = pPlayer->GetEquipment(Equip_Weapon);
	if (EquipItem)
		cout << EquipItem->GetName() << endl;
	else
		cout << "없음";
	cout << endl;

	cout << "장착 방어구 : ";
	EquipItem = pPlayer->GetEquipment(Equip_Armor);
	if (EquipItem)
		cout << EquipItem->GetName() << endl;
	else
		cout << "없음";
	cout << endl;


	cout << "장착할 아이템을 선택하세요 :";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu >= m_Size) return -1;
	return _Menu;
}
CItem* CInventory::Equip(CItem* NewItem)
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	CItem* Item = pPlayer->Equip(NewItem);
	// 장착 시키기 
	return Item;
}


