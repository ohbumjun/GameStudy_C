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
		// ������� ���� ���� ������ ��
		CInventory::GetInst()->Equip(m_Item[Index]->CloneItem());
		// �� ����
		pPlayer->AddGold(-m_Item[Index]->GetPrice());
	}
}

int CStore::Menu(Store_Type Type)
{
	switch (Type)
	{
	case ST_Weapon:
		cout << "==== ���ⱸ ���� ====" << endl;
		break;
	case ST_Armor:
		cout << "==== �� ���� ====" << endl;
		break;
	}
	for (int i = 0; i < IT_End; i++)
	{
		cout << i+1 << ". " ;
		m_Item[i]->Output();
		cout << endl;
	}
	cout << "4. �ڷ� ����" << endl;
	cout << "�������� �����ϼ��� : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > 4) return -1;
	return _Menu;
}
