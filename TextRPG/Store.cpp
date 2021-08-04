#include "Store.h"
#include "Player.h"
#include "Item.h"
#include"ObjectManager.h"
#include "Inventory.h"

using namespace std;

CStore::CStore() :
	m_Item{}
{
}

CStore::~CStore()
{
	for (int i = 0; i < IT_End; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

CItem* CStore::CreateItem(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;
	Item->Init(Name, Type,Option,Price,Sell,Desc);
	return Item;
}

bool CStore::Init(const Store_Type& Type)
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

void CStore::SetWeaponList()
{
	m_Item[0] = CreateItem("목검", IT_Weapon, 50, 1000, 500, "나무로 만든 검");
	m_Item[1] = CreateItem("BF.대검", IT_Weapon, 200, 8000, 4000, "조금 센 검");
	m_Item[2] = CreateItem("무한의대검", IT_Weapon, 600, 40000, 20000, "치명타 맞고 죽어라.");

}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("천갑옷", IT_Armor, 25, 1000, 500, "약한 방어구");
	m_Item[1] = CreateItem("거인의 벨트", IT_Armor, 100, 8000, 4000, "조금 단단한 벨트");
	m_Item[2] = CreateItem("가시갑옷", IT_Armor, 300, 40000, 20000, "구른다!!");

}

void CStore::Run()
{
	CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
	while (true)
	{
		int Select = Menu(pPlayer);
		if (Select == 0) continue;
		if (Select == 3) return;
		int Index = Select - 1;
		if (pPlayer->GetGold() < m_Item[Index]->GetPrice())
		{
			cout << "금액이 부족합니다." << endl;
			system("pause");
			continue;
		}
		else if (CInventory::GetInst()->IsFull())
		{
			cout << "인벤토리 창이 가득찼습니다" << endl;
			system("pause");
			continue;
		}
		CItem* CloneItem = m_Item[Index]->Clone();
		// 플레이어 돈 감소
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
		// 인벤토리 추가 
		CInventory::GetInst()->AddItem(CloneItem);
		cout << "구매완료" << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* pPlayer)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "==== 무기상점 ====" << endl;
		break;
	case ST_Armor:
		cout << "==== 방어구상점 ====" << endl;
		break;
	}

	for (int i = 0; i < IT_End; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}

	cout << "3. 뒤로 가기" << endl;
	cout << "보유 금액 : " << pPlayer->GetGold() << endl;
	cout << "아이템을 선택해주세요 :";
	
	int Item;
	cin >> Item;

	if (Item <= (int)Store_Menu::None || Item > (int)Store_Menu::Back) return 0;
	return Item;
}
