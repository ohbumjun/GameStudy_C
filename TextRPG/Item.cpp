#include "Item.h"

using namespace std;

CItem::CItem() :
	m_Name{},
	m_Type(Item_Type::IT_Weapon),
	m_Option(0),
	m_Price(0),
	m_Sell(0),
	m_Desc{}
{
}

CItem::CItem(CItem& Item)
{
	*this = Item;
}

CItem::~CItem()
{
}

CItem* CItem::CloneItem()
{
	return new CItem(*this);
	return nullptr;
}

void CItem::Output()
{
	cout << "�̸� : " << m_Name << endl;
	switch (m_Type)
	{
	case IT_Weapon:
		cout << "���ݷ� : ";
		break;
	case IT_Armor:
		cout << "���� : ";
		break;
	}
	cout << m_Option ;
	cout << endl;
	cout << "���� : " << m_Price << endl;
	cout << "�ǸŰ� : " << m_Sell << endl;
	cout << "���� : " << m_Desc << endl;
}

bool CItem::Init(const char* Name, Item_Type Type,
	int Option, int Price, int Sell, const char* Desc)
{
	strcpy_s(m_Name, Name);
	m_Type = Type;
	m_Option = Option;
	m_Price = Price;
	m_Sell = Sell;
	strcpy_s(m_Desc, Desc);
	return false;
}
