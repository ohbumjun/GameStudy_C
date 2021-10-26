#include "MapManager.h"
#include "Map.h"

using namespace std;

CMapManager* CMapManager::m_pInst = nullptr;
CMapManager::CMapManager() : m_Map{}
{
}
CMapManager::~CMapManager()
{
    for (int i = 0; i < MT_End; i++)
    {
        if (m_Map[i]) delete m_Map[i];
    }
}
void CMapManager::Run()
{
    while (true)
    {
        int Select = Menu();
        if (Select == -1) continue;
        if (Select == 0) return;
        int Index = Select - 1;
        m_Map[Index]->Run();
    }
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

int CMapManager::Menu()
{
    system("cls");
    cout << "1. 쉬움" << endl;
    cout << "2. 중간" << endl;
    cout << "3. 어려움" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "맵을 선택하세요 :";

    int _Menu;
    cin >> _Menu;

    if (_Menu < (int)Map_Menu::None || _Menu >(int)Map_Menu::Hard)
        return -1;

    return _Menu;
}
