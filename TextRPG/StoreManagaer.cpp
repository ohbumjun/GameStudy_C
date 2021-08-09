#include "StoreManagaer.h"
#include "Store.h"

using namespace std;

CStoreManagaer* CStoreManagaer::m_pInst = nullptr;

CStoreManagaer::CStoreManagaer() : m_Store{}
{
}

CStoreManagaer::~CStoreManagaer()
{
    for (int i = 0; i < ST_End; i++)
    {
        if (m_Store[i]) delete m_Store[i];
    }
}

void CStoreManagaer::Run()
{
    while (true)
    {
        switch (Menu())
        {
        case Store_Menu::None:
            break;
        case Store_Menu::Weapon:
            break;
        case Store_Menu::Armor:
            break;
        case Store_Menu::Back:
            break;
        default:
            break;
        }
    }
};

Store_Menu CStoreManagaer::Menu()
{
    system("cls");
    cout << "1. 무기상점" << endl;
    cout << "2. 방어구상점" << endl;
    cout << "3. 뒤로가기" << endl;
    cout << "메뉴를 선택하세요 :";
    int _Menu;
    cin >> _Menu;
    if (_Menu <= (int)Store_Menu::None || _Menu > (int)Store_Menu::Back)
        return Store_Menu::None;
    return (Store_Menu)_Menu;
}

bool CStoreManagaer::Init()
{
    m_Store[0] = new CStore;
    if(!m_Store[0]->Init(ST_Weapon)) return false;
    m_Store[1] = new CStore;
    if (!m_Store[1]->Init(ST_Weapon)) return false;
    return true;
}
