#include "StoreManager.h"
#include "Store.h"

using namespace std;

CStoreManager* CStoreManager::m_pInst = nullptr;

CStoreManager::CStoreManager() : m_Store{}
{
}

CStoreManager::~CStoreManager()
{
	for (int i = 0; i < ST_End; i++)
	{
		if (m_Store[i]) delete m_Store[i];
	}
}

bool CStoreManager::Init()
{
	m_Store[ST_Weapon] = new CStore;
	if(!m_Store[ST_Weapon]->Init(ST_Weapon)){}
	m_Store[ST_Armor] = new CStore;
	if(!m_Store[ST_Armor]->Init(ST_Armor)){}
	return true;
}

void CStoreManager::Run()
{
	switch (Menu())
	{
	case Store_Menu::Weapon:
		m_Store[ST_Weapon]->Run();
		break;
	case Store_Menu::Armor:
		m_Store[ST_Weapon]->Run();
		break;
	}
}

Store_Menu CStoreManager::Menu()
{
	cout << "1. 무기구 상점" << endl;
	cout << "2. 방어구 상점" << endl;
	cout << "상점을 선택하세요 :" ;
	int  _Menu;
	cin >> _Menu;
	if (_Menu < (int)Store_Menu::Weapon || _Menu >(int)Store_Menu::Armor) return Store_Menu::None;
	return (Store_Menu)_Menu;
}
