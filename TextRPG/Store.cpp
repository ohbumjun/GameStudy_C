#include "Store.h"
#include "Item.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Inventory.h"

using namespace std;

CStore::CStore() : m_Item{}, m_Type(Store_Type::None)
{
}

CStore::~CStore()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

bool CStore::Init(Store_Type Type)
{
	m_Type = Type;
	switch (m_Type)
	{
	case ST_Weapon:
		SetWeaponList();
		break;
	case ST_Armor:
		SetArmorList();
		break;
	}
	return false;
}

void CStore::SetWeaponList()
{
	m_Item[0] = CreateItem("나무검", IT_Weapon, 200, 1000, 500, "나무로 만든 검");
	m_Item[1] = CreateItem("철검", IT_Weapon, 500, 2000, 1000, "철로 만든 검");
	m_Item[2] = CreateItem("황금검", IT_Weapon, 2000, 4000, 2000, "금으로 만든 검");
}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("나무방패", IT_Armor, 200, 1000, 500, "나무로 만든 방패");
	m_Item[1] = CreateItem("철방패", IT_Armor, 500, 2000, 1000, "철로 만든 방패");
	m_Item[2] = CreateItem("황금방패", IT_Armor, 2000, 4000, 2000, "금으로 만든 방패");
}

CItem* CStore::CreateItem(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name, Type, Option, Price, Sell, Desc);
	return Item;
}

void CStore::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(pPlayer);
		if (Select == -1) continue;
		if (Select == 0) return;
		int Index = Select - 1;

		// 아이템을 구매할 수 없는 경우
		// 1) 돈이 없을 때
		if (pPlayer->GetGold() < m_Item[Index]->GetPrice())
		{
			cout << "돈이 없어요" << endl;
			system("pause");
			continue;
		}
		// 2) 장비창이 가득 찼을 때
		if (CInventory::GetInst()->IsFull())
		{
			cout << "장비창이 가득 찼습니다" << endl;
			system("pause");
			continue;
		}
		// 여기까지 오면 아이템 구매 가능
		CItem* ItemBought = m_Item[Index];
		// 장바칸에 추가
		CInventory::GetInst()->AddItem(ItemBought->Clone());
		// 돈 감소 
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
		// 표시 
		cout << "구매완료" << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* pPlayer)
{
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "=== 무기 상점 ====" << endl;
		break;
	case ST_Armor:
		cout << "==== 방어구 상점 ====" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "0. 뒤로가기" << endl;

	int _Menu;
	cin >> _Menu;

	if (_Menu < 0 || _Menu >= 4) return -1;
	return _Menu;
}
