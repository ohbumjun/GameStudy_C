#include "MapManager.h"
#include "Map.h"

using namespace std;

CMapManager* CMapManager::m_pInst = nullptr;
CMapManager::CMapManager() : m_Maps{}
{
}
CMapManager::~CMapManager()
{
	for (int i = 0; i < MT_End; i++)
	{
		if (m_Maps[i]) delete m_Maps[i];
	}
}

Map_Menu CMapManager::Menu()
{
	system("cls");
	int _Menu ;
	cout << "1. 쉬움" << endl;
	cout << "2. 보통" << endl;
	cout << "3. 어려움" << endl;
	cout << "4. 뒤로 가기 ";
	cin >> _Menu;
	if (_Menu <= (int)Map_Menu::None || _Menu > (int)Map_Menu::End)
		return Map_Menu::None;
	return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
	m_Maps[0] = new CMap;
	if (!m_Maps[0]->Init(MT_Easy)) return false;
	m_Maps[1] = new CMap;
	if (!m_Maps[1]->Init(MT_Normal)) return false;
	m_Maps[2] = new CMap;
	if (!m_Maps[2]->Init(MT_Hard)) return false;
	return true;
}

void CMapManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Map_Menu::Easy:
			m_Maps[0]->Run();
			break;
		case Map_Menu::Normal:
			m_Maps[1]->Run();
			break;
		case Map_Menu::Hard:
			m_Maps[2]->Run();
			break;
		case Map_Menu::End:
			return;
		}
	}
}
