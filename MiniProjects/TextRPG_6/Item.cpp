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
	cout << "�̸� : " << m_Name << endl;
	cout << "���� : " << m_Price << endl;
	cout << "�ɼ� : " << m_Option << endl;
	cout << "�ǸŰ� : " << m_Sell << endl;
	cout << "���� : " << m_Desc << endl;
}

CItem* CItem::Clone()
{
	return new CItem(*this);
}
