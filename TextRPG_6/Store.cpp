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
		m_Items[0]->Init("����Į", IT_Weapon, 2000, 200, 200, "�����θ���Į");
		m_Items[1] = new CItem;
		m_Items[1]->Init("öĮ", IT_Weapon, 4000, 400, 400, "ö�θ���Į");
		m_Items[2] = new CItem;
		m_Items[2]->Init("��Į", IT_Weapon, 2000, 600, 600, "�����θ���Į");
		break;
	case ST_Armor:
		m_Items[0] = new CItem;
		m_Items[0]->Init("��������", IT_Armor, 2000, 200, 200, "�����θ������");
		m_Items[1] = new CItem;
 		m_Items[1]->Init("ö����", IT_Armor, 4000, 400, 400, "ö�θ������");
		m_Items[2] = new CItem;
		m_Items[2]->Init("�ݹ���", IT_Armor, 6000, 800, 800, "�����θ������");
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
		
		// �� �� ���� ���
		// 1) �κ��丮 ���� ��
		if (CInventory::GetInst()->IsFull())
		{
			cout << "�κ��丮�� ����á���ϴ�" << endl;
			system("pause");
			continue;
		}
		// 2) ���� ����
		if (Player->GetGold() < CloneItem->GetPrice())
		{
			cout << "���� �����մϴ�" << endl;
			system("pause");
			continue;
		}
		CInventory::GetInst()->AddItem(CloneItem);
		cout << CloneItem->GetName() << " ����! " << endl;
		system("pause");
	}
}

int CStore::Menu(CPlayer* player)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		cout << "---- ���� ���� ----" << endl;
		break;
	case ST_Armor:
		cout << "---- �� ���� ----" << endl;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		cout << i+1 << ". " << endl;
		m_Items[i]->Output();
		cout << endl;
	}
	cout << "0 . �ڷΰ���" << endl;
	cout << "���� �ݾ� : " << player->GetGold() << endl;
	cout << "������ ���⸦ �����ϰԿ� : " ;
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > IT_End) return -1;
	return _Menu;
}
