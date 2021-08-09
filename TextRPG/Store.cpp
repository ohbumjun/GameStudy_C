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
		// ������ �� ���� ����� ���� ����ؾ� �Ѵ�
		// 1) ������ â�� ��� á�� ��
		if (CInventory::GetInst()->IsFull())
		{
			cout << "�κ��丮 â�� ����á���ϴ�" << endl;
			system("pause");
			continue;
		}
		// 2) ���� ���� �� 
		if (pPlayer->GetGold() < m_Item[Index]->GetPrice())
		{
			cout << "���� �����." << endl;
			system("pause");
			continue;
		}
		// ������ ����
		cout << "Name : " << m_Item[Index]->GetName() << endl;
		CItem* CloneItem = m_Item[Index]->CloneItem();
		// ������� ���� ���� ������ ��
		// �� ����
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
		cout << "==== ���ⱸ ���� ====" << endl;
		break;
	case ST_Armor:
		cout << "==== �� ���� ====" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i+1 << ". " ;
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "4. �ڷ� ����" << endl;
	cout << "�����ݾ� : " << pPlayer->GetGold() << endl;
	cout << "�������� �����ϼ��� : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > 4) return -1;
	return _Menu;
}

void CStore::SetWeaponList()
{
	m_Item[0] = CreateItem("���", IT_Weapon, 50, 1000, 500, "������ ���� ��");
	m_Item[1] = CreateItem("BF.���", IT_Weapon, 200, 8000, 4000, "���� �� ��");
	m_Item[2] = CreateItem("���Ѵ��", IT_Weapon, 600, 40000, 20000, "ġ��Ÿ �׾��.");

}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("õ����", IT_Armor, 25, 1000, 500, "���� ��");
	m_Item[1] = CreateItem("������ ��Ʈ", IT_Armor, 100, 8000, 4000, "���� �ܴ��� ��Ʈ");
	m_Item[2] = CreateItem("������", IT_Armor, 300, 40000, 20000, "������!!");

}
