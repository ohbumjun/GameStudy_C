#include "Item.h"

using namespace std;

CItem::CItem() :
	m_Name{},
	m_Type(IT_Weapon),
	m_Price(0),
	m_Option(0),
	m_Sell(0),
	m_Desc{}
{

}

CItem::CItem(const CItem* Item)
{
	*this = Item;
}

CItem::~CItem()
{
}

bool CItem::Init(const char* Name, Item_Type Type, int Price, int Option, int Sell, const char* Desc)
{
	strcpy_s(m_Name, Name);
	m_Type = Type;
	m_Price = Price;
	m_Option = Option;
	m_Sell = Sell;
	strcpy_s(m_Desc, Desc);

    return true;
}

void CItem::Output()
{
	cout << "이름 : " << m_Name << endl;
	cout << "가격 : " << m_Price << endl;
	cout << "옵션 : " << m_Option << endl;
	cout << "판매가 : " << m_Sell << endl;
	cout << "설명 : " << m_Desc << endl;
}

CItem* CItem::Clone()
{
	return new CItem(*this);
}
