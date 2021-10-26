#include "Store.h"
#include "ObjectManager.h"
#include "Item.h"
#include "Player.h"
#include "Inventory.h"

using namespace std;

CStore::CStore() :
	m_Items{}
{
}

CStore::~CStore()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Items[i]) delete m_Items[i];
	}
}

bool CStore::Init(Store_Type Type)
{
	m_Type = Type;
	switch (m_Type)
	{
	case ST_Weapon:
		m_Items[0] = new CItem;
		m_Items[0]->Init("나무칼", IT_Weapon, 2000, 200, 200, "나무로만든칼");
		m_Items[1] = new CItem;
		m_Items[1]->Init("철칼", IT_Weapon, 4000, 400, 400, "철로만든칼");
		m_Items[2] = new CItem;
		m_Items[2]->Init("금칼", IT_Weapon, 2000, 600, 600, "금으로만든칼");
		break;
	case ST_Armor:
		m_Items[0] = new CItem;
		m_Items[0]->Init("나무방패", IT_Armor, 2000, 200, 200, "나무로만든방패");
		m_Items[1] = new CItem;
 		m_Items[1]->Init("철방패", IT_Armor, 4000, 400, 400, "철로만든방패");
		m_Items[2] = new CItem;
		m_Items[2]->Init("금방패", IT_Armor, 6000, 800, 800, "금으로만든방패");
		break;
	}
	return true;
}

void CStore::Run()
{
	CPlayer* Player = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(Player);
		if (Select == 0) return;
		if (Select == -1) continue;
		int Index = Select - 1;

		CItem* CloneItem = m_Items[Index]->Clone();
		
		// 살 수 없는 경우
		// 1) 인벤토리 가득 참
		if (CInventory::GetInst()->IsFull())
		{
			cout << "인벤토리가 가득찼습니다" << endl;
			system("pause");
			continue;
		}
		// 2) 돈이 부족
		if (Player->GetGold() < CloneItem->GetPrice())
		{
			cout << "돈이 부족합니다" << endl;
			system("pause");
			continue;
		}
		CInventory::GetInst()->AddItem(CloneItem);
		cout << CloneItem->GetName() << " 구입! " << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* player)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "---- 무기 상점 ----" << endl;
		break;
	case ST_Armor:
		cout << "---- 방어구 상점 ----" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i+1 << ". " << endl;
		m_Items[i]->Output();
		cout << endl;
	}
	cout << "0 . 뒤로가기" << endl;
	cout << "보유 금액 : " << player->GetGold() << endl;
	cout << "구입할 무기를 선택하게요 : " ;
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > IT_End) return -1;
	return _Menu;
}
