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
	m_Item[0] = CreateItem("������", IT_Weapon, 200, 1000, 500, "������ ���� ��");
	m_Item[1] = CreateItem("ö��", IT_Weapon, 500, 2000, 1000, "ö�� ���� ��");
	m_Item[2] = CreateItem("Ȳ�ݰ�", IT_Weapon, 2000, 4000, 2000, "������ ���� ��");
}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("��������", IT_Armor, 200, 1000, 500, "������ ���� ����");
	m_Item[1] = CreateItem("ö����", IT_Armor, 500, 2000, 1000, "ö�� ���� ����");
	m_Item[2] = CreateItem("Ȳ�ݹ���", IT_Armor, 2000, 4000, 2000, "������ ���� ����");
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

		// �������� ������ �� ���� ���
		// 1) ���� ���� ��
		if (pPlayer->GetGold() < m_Item[Index]->GetPrice())
		{
			cout << "���� �����" << endl;
			system("pause");
			continue;
		}
		// 2) ���â�� ���� á�� ��
		if (CInventory::GetInst()->IsFull())
		{
			cout << "���â�� ���� á���ϴ�" << endl;
			system("pause");
			continue;
		}
		// ������� ���� ������ ���� ����
		CItem* ItemBought = m_Item[Index];
		// ���ĭ�� �߰�
		CInventory::GetInst()->AddItem(ItemBought->Clone());
		// �� ���� 
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
		// ǥ�� 
		cout << "���ſϷ�" << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* pPlayer)
{
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "=== ���� ���� ====" << endl;
		break;
	case ST_Armor:
		cout << "==== �� ���� ====" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i + 1 << ". ";
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "0. �ڷΰ���" << endl;

	int _Menu;
	cin >> _Menu;

	if (_Menu < 0 || _Menu >= 4) return -1;
	return _Menu;
}
