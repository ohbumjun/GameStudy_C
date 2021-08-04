#include "StoreManager.h"
#include "Store.h"

using namespace std;

CStoreManager* CStoreManager::m_pInst = nullptr;
CStoreManager::CStoreManager() 
	: m_Store{}
{
}

CStoreManager::~CStoreManager()
{
	for (int i = 0; i < ST_End; i++)
	{
		if (m_Store[i])
			delete m_Store[i];
	}
}

Store_Menu CStoreManager::Menu()
{
	system("cls");
	cout << "1. �������" << endl;
	cout << "2. ������" << endl;
	cout << "3. �ڷΰ���" << endl;
	cout << "������ �����ϼ��� : ";
	int _Menu;
	cin >> _Menu;

	if (_Menu <= (int)Store_Menu::None || _Menu > (int)Store_Menu::Back)
		return Store_Menu::None;
	return Store_Menu(_Menu);
}

bool CStoreManager::Init()
{
	m_Store[ST_Weapon] = new CStore;

	if (!m_Store[ST_Weapon]->Init(ST_Weapon))
		return false;

	m_Store[ST_Armor] = new CStore;

	if (!m_Store[ST_Armor]->Init(ST_Armor))
		return false;
	return true;
}

void CStoreManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Store_Menu::Weapon:
			m_Store[ST_Weapon]->Run();
			break;
		case Store_Menu::Armor:
			m_Store[ST_Armor]->Run();
			break;
		case Store_Menu::Back:
			return;
		}
	}
}
