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

CItem::~CItem()
{
}

void CItem::Output()
{
	cout << "이름 : " << m_Name << endl;
	cout << "종류 : ";
	switch (m_Type)
	{
	case IT_Weapon:
		cout << "무기 " << endl;
		cout << "공격력 : " << m_Option << endl;
		break;
	case IT_Armor:
		cout << "방어 " << endl;
		cout << "방어력 : " << m_Option << endl;
		break;
	}
	cout << "옵션 : " << m_Option << endl;
	cout << "가격 : " << m_Price << endl;
	cout << "판매가 : " << m_Sell << endl;
	cout << "설명 : " << m_Desc << endl;
}


bool CItem::Init(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	strcpy_s(m_Name,Name);
	m_Type = Type;
	m_Option = Option;
	m_Price = Price;
	m_Sell = Sell;
	strcpy_s(m_Desc, Desc);

	return true;
}

CItem* CItem::Clone()
{
	return new CItem(*this);
}
