#include "Inventory.h"
#include "Item.h"
#include "ObjectManager.h"
#include "Player.h"

using namespace std;

CInventory* CInventory::m_pInst = nullptr;
CInventory::CInventory() : m_Count(0),m_Item{}
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Count; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

void CInventory::Run()
{
	while (true)
	{
		int Select = Menu();
		if (Select == -1) continue;
		if (Select == 0) break;
		int Index = Select - 1;
		CPlayer* player = CObjectManager::GetInst()->GetPlayer();
		CItem* EquipItem = player->Equip(m_Item[Index]);
		if (EquipItem)
		{
			m_Item[Index] = EquipItem;
		}
		else
		{
			for (int i = Index; i < m_Count; i++)
			{
				m_Item[i] = m_Item[i + 1];
			}
			m_Item[m_Count] = nullptr;
			--m_Count;
		}
	}
}

int CInventory::Menu()
{
	cout << "==== 인벤토리 ====" << endl;
	for (int i = 0; i < m_Count; i++)
	{
		cout << i + 1 << ". " << endl;
		m_Item[i]->Output();
	}
	cout << "0. 뒤로가기" << endl;
	cout << "장착할 아이템을 선택하세요" ;
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > m_Count) return -1;
	return _Menu;
}

bool CInventory::Init()
{
	return true;
}
