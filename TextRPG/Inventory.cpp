#include "Inventory.h"
#include "Item.h"
#include "Player.h"
#include "ObjectManager.h"

using namespace std;

CInventory* CInventory::m_pInst = nullptr;
CInventory::CInventory() :
	m_Item{},
	m_Count(0)
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Count; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

bool CInventory::Init()
{

	return true;
}

int CInventory::Menu(CPlayer* pPlayer)
{
	system("cls");
	cout << "==== 인벤토리 ====" << endl;
	for (int i = 0; i < m_Count; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "0.뒤로 가기" << endl;
	
	cout << "장착 무기 : ";
	if (pPlayer->GetEquipItem(Equip_Weapon))
		cout << pPlayer->GetEquipItem(Equip_Weapon)->GetName() << endl;
	else  cout << "없음 " << endl;
	cout << "장착 보호구 : ";
	if (pPlayer->GetEquipItem(Equip_Armor))
		cout << pPlayer->GetEquipItem(Equip_Armor)->GetName() << endl;
	else  cout << "없음 " << endl;
	
	cout << "장착할 아이템을 선택하세요 :";
	int _Menu;
	cin >> _Menu;

	if (_Menu < 0 || _Menu > m_Count )
		return -1;

	return _Menu;
}

void CInventory::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(pPlayer);
		if (Select == 0) return;
		if (Select == -1) continue;
		int Index = Select - 1;
		CItem* EquipItem = pPlayer->Equip(m_Item[Index]);
		if (EquipItem) m_Item[Index] = EquipItem;
		else
		{
			for (int i = Index; i < m_Count -1; i++)
			{
				m_Item[i] = m_Item[i + 1];
			}
			m_Item[m_Count-1] = nullptr;
			--m_Count;
		}
	}
}
