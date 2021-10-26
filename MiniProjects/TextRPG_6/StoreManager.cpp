#include "StoreManager.h"
#include "Store.h"

using namespace std;

CStoreManager* CStoreManager::m_pInst = nullptr;
CStoreManager::CStoreManager() :
	m_Store{}
{
}
CStoreManager::~CStoreManager()
{
	for (int i = 0; i < ST_End; i++)
		delete m_Store[i];
}
void CStoreManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Store_Menu::None:
			return;
		case Store_Menu::Weapon:
			m_Store[0]->Run();
			break;
		case Store_Menu::Armor:
			m_Store[1]->Run();
			break;
		}
	}
}

bool CStoreManager::Init()
{
	m_Store[0] = new CStore;
	if (!m_Store[0]->Init(ST_Weapon)) return false;
	m_Store[1] = new CStore;
	if (!m_Store[1]->Init(ST_Armor)) return false;
	return true;
}

Store_Menu CStoreManager::Menu()
{
	system("cls");
	cout << "1. 무기상점" << endl;
	cout << "2. 방어구상점" << endl;
	cout << "0. 뒤로 가기" << endl;
	cout << "상점을 선택하세요 : ";
	int _Menu;
	cin >> _Menu;

	if ((Store_Menu)_Menu <= Store_Menu::None || (Store_Menu)_Menu > Store_Menu::Armor)
		return Store_Menu::None;

	return (Store_Menu)_Menu;
}
