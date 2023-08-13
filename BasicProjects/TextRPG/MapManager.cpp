#include "MapManager.h"
#include "Map.h"

using namespace std;

CMapManager* CMapManager::m_pInst = nullptr;
CMapManager::CMapManager() : m_Map{}
{
}

CMapManager::~CMapManager()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Map[i]) delete m_Map[i];
	}
}

void CMapManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Map_Menu::Easy:
			m_Map[0]->Run();
			break;
		case Map_Menu::Normal:
			m_Map[1]->Run();
			break;
		case Map_Menu::Hard:
			m_Map[2]->Run();
			break;
		case Map_Menu::End:
			return;
		}
	}
}

Map_Menu CMapManager::Menu()
{
	system("cls");
	cout << "1.쉬움" << endl;
	cout << "2.보통" << endl;
	cout << "3.어려움" << endl;
	cout << "4.뒤로가기" << endl;
	cout << "맵을 선택하세요 : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu < 0 || _Menu > 4) return Map_Menu::None;

	return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
	m_Map[0] = new CMap;
	if (!m_Map[0]->Init(MT_Easy)) return false;
	m_Map[1] = new CMap;
	if (!m_Map[1]->Init(MT_Normal)) return false;
	m_Map[2] = new CMap;
	if (!m_Map[2]->Init(MT_Hard)) return false;
	return true;
}
