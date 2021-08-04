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
	m_Item[0] = CreateItem("���", IT_Weapon, 50, 1000, 500, "������ ���� ��");
	m_Item[1] = CreateItem("BF.���", IT_Weapon, 200, 8000, 4000, "���� �� ��");
	m_Item[2] = CreateItem("�����Ǵ��", IT_Weapon, 600, 40000, 20000, "ġ��Ÿ �°� �׾��.");

}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("õ����", IT_Armor, 25, 1000, 500, "���� ��");
	m_Item[1] = CreateItem("������ ��Ʈ", IT_Armor, 100, 8000, 4000, "���� �ܴ��� ��Ʈ");
	m_Item[2] = CreateItem("���ð���", IT_Armor, 300, 40000, 20000, "������!!");

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
			cout << "�ݾ��� �����մϴ�." << endl;
			system("pause");
			continue;
		}
		else if (CInventory::GetInst()->IsFull())
		{
			cout << "�κ��丮 â�� ����á���ϴ�" << endl;
			system("pause");
			continue;
		}
		CItem* CloneItem = m_Item[Index]->Clone();
		// �÷��̾� �� ����
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
		// �κ��丮 �߰� 
		CInventory::GetInst()->AddItem(CloneItem);
		cout << "���ſϷ�" << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* pPlayer)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "==== ������� ====" << endl;
		break;
	case ST_Armor:
		cout << "==== ������ ====" << endl;
		break;
	}

	for (int i = 0; i < IT_End; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}

	cout << "3. �ڷ� ����" << endl;
	cout << "���� �ݾ� : " << pPlayer->GetGold() << endl;
	cout << "�������� �������ּ��� :";
	
	int Item;
	cin >> Item;

	if (Item <= (int)Store_Menu::None || Item > (int)Store_Menu::Back) return 0;
	return Item;
}
