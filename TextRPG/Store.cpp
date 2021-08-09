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
	switch (m_Type)
	{
	case ST_Weapon:
		SetWeaponList();
		break;
	case ST_Armor:
		SetArmorList();
		break;
	}
	return true;
}

void CStore::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(pPlayer);
		if (Select == 4) break;
		if (Select == -1) continue;
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
		// 아이템 복사
		cout << "Name : " << m_Item[Index]->GetName() << endl;
		CItem* CloneItem = m_Item[Index]->CloneItem();
		// 여기까지 오면 구매 가능한 것
		// 돈 감소
		CInventory::GetInst()->AddItem(CloneItem);
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
		system("pause");
	}
}

CItem* CStore::CreateItem(const char* Name, Item_Type Type,
	int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name, Type, Option, Price, Sell, Desc);
	return Item;
}

int CStore::Menu(CPlayer* pPlayer)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "==== 무기구 상점 ====" << endl;
		break;
	case ST_Armor:
		cout << "==== 방어구 상점 ====" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i+1 << ". " ;
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "4. 뒤로 가기" << endl;
	cout << "보유금액 : " << pPlayer->GetGold() << endl;
	cout << "아이템을 선택하세요 : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > 4) return -1;
	return _Menu;
}

void CStore::SetWeaponList()
{
	m_Item[0] = CreateItem("목검", IT_Weapon, 50, 1000, 500, "나무로 만든 검");
	m_Item[1] = CreateItem("BF.대검", IT_Weapon, 200, 8000, 4000, "조금 센 검");
	m_Item[2] = CreateItem("무한대검", IT_Weapon, 600, 40000, 20000, "치명타 죽어라.");

}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("천갑옷", IT_Armor, 25, 1000, 500, "약한 방어구");
	m_Item[1] = CreateItem("거인의 벨트", IT_Armor, 100, 8000, 4000, "조금 단단한 벨트");
	m_Item[2] = CreateItem("가갑옷", IT_Armor, 300, 40000, 20000, "구른다!!");

}
