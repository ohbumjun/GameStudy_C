#include "Inventory.h"
#include "Item.h"
#include "Player.h"
#include "ObjectManager.h"

CInventory* CInventory::m_pInst = nullptr;

CInventory::CInventory() :
	m_Item{},
	m_Count(0)
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Count; ++i)
	{
		delete	m_Item[i];
	}
}

int CInventory::Menu(CPlayer* Player)
{
	system("cls");

	std::cout << "======================= �κ��丮 =======================" << std::endl;
	for (int i = 0; i < m_Count; ++i)
	{
		std::cout << i + 1 << ". ";
		m_Item[i]->Output();
		std::cout << std::endl;
	}

	std::cout << "0. �ڷΰ���" << std::endl;
	std::cout << "�������� : ";

	CItem* EquipItem = Player->GetEquipment(Equip_Weapon);

	if (EquipItem)
		std::cout << EquipItem->GetName();

	else
		std::cout << "����";

	std::cout << "\t������ : ";
	EquipItem = Player->GetEquipment(Equip_Armor);

	if (EquipItem)
		std::cout << EquipItem->GetName() << std::endl;

	else
		std::cout << "����" << std::endl;

	std::cout << "������ �������� �����ϼ��� : ";
	int	Item = 0;
	std::cin >> Item;

	if (Item < 0 || Item > m_Count)
		return -1;

	return Item;
}

bool CInventory::Init()
{
	return true;
}

void CInventory::Run()
{
	CPlayer* Player = CObjectManager::GetInst()->GetPlayer();

	while (true)
	{
		int	Select = Menu(Player);

		if (Select == 0)
			break;

		else if (Select == -1)
			continue;

		int	Index = Select - 1;

		CItem* EquipItem = Player->Equip(m_Item[Index]);

		if (EquipItem)
			m_Item[Index] = EquipItem;

		else
		{
			// ������Ų ������ �ڿ� �ִ� �����۵��� ��� 1ĭ�� ������ �̵������ش�.
			for (int i = Index; i < m_Count - 1; ++i)
			{
				m_Item[i] = m_Item[i + 1];
			}

			// ���� ������ ĭ�� nullptr�� ���ش�.
			m_Item[m_Count - 1] = nullptr;

			--m_Count;
		}
	}
}
