#include "MapManager.h"
#include"Map.h"

using namespace std;

CMapManager* CMapManager::m_pInst = nullptr;
CMapManager::CMapManager() :
    m_pMap{}
{
}

CMapManager::~CMapManager()
{
    for (int i = 0; i < MT_End; i++)
    {
        if (m_pMap[i]) delete m_pMap[i];
    }
}

Map_Menu CMapManager::Menu()
{
    system("cls");

    cout << "1. 쉬움" << endl;
    cout << "2. 보통" << endl;
    cout << "3. 어려움" << endl;
    cout << "4. 뒤로가기" << endl;
    cout << "맵을 선택하세요 " << endl;
    int _Menu;
    std::cin >> _Menu;
    if (_Menu < (int)Map_Menu::None || _Menu >(int)Map_Menu::Back) return Map_Menu::None;
    return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
    m_pMap[0] = new CMap;
    if (!m_pMap[0]->Init(MT_Easy)) return false;
    m_pMap[1] = new CMap;
    if (!m_pMap[1]->Init(MT_Normal)) return false;
    m_pMap[2] = new CMap;
    if (!m_pMap[2]->Init(MT_Hard)) return false;
    return true;
}

void CMapManager::Run()
{
    cout << "map run" << endl;
    while (true)
    {
        switch (Menu())
        {
        case Map_Menu::Easy:
            m_pMap[0]->Run();
            break;
        case Map_Menu::Normal:
            m_pMap[1]->Run();
            break;
        case Map_Menu::Hard:
            m_pMap[2]->Run();
            break;
        case Map_Menu::Back:
            return;
        }
    }
}

