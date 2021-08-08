#include "Store.h"
#include "Item.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Inventory.h"

using namespace std;

CStore::CStore() : m_Item{}, m_Type(ST_Weapon)
{
}

CStore::~CStore()
{
	for (int i = 0; i < ST_End; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

bool CStore::Init(Store_Type Type)
{
	m_Type = Type;
	return true;
}

void CStore::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(m_Type);
		if (Select == 4) continue;
		if (Select == -1) return;
		int Index = Select - 1;
		// 구매할 수 없는 경우의 수를 고려해야 한다
		// 1) 아이템 창이 모두 찼을 때
		if (CInventory::GetInst()->IsFull())
		{
			cout << "인벤토리 창이 가득찼습니다" << endl;
			system("pause");
			continue;
		}
		// 2) 돈이 없을 때 
		if (pPlayer->GetGold() < m_Item[Index]->GetPrice())
		{
			cout << "돈이 없어요." << endl;
			system("pause");
			continue;
		}
		// 여기까지 오면 구매 가능한 것
		CInventory::GetInst()->Equip(m_Item[Index]->CloneItem());
		// 돈 감소
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
	}
}

int CStore::Menu(Store_Type Type)
{
	switch (Type)
	{
	case ST_Weapon:
		cout << "==== 무기구 상점 ====" << endl;
		break;
	case ST_Armor:
		cout << "==== 방어구 상점 ====" << endl;
		break;
	}
	for (int i = 0; i < IT_End; i++)
	{
		cout << i+1 << ". " ;
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "4. 뒤로 가기" << endl;
	cout << "아이템을 선택하세요 : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > 4) return -1;
	return _Menu;
}
