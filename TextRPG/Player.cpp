#include "Player.h"
#include "Item.h"
#include "ObjectManager.h"

using namespace std;

CPlayer::CPlayer() :
	m_Name{},
	m_Job(Job::Archer),
	m_Attack(0),
	m_Armor(0),
	m_HP(0),
	m_HPMax(0),
	m_MP(0),
	m_MPMax(0),
	m_Exp(0),
	m_Gold(0),
	m_Level(0),
	m_Equipment{}
{
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < Equip_End; i++)
	{
		if (m_Equipment[i]) delete m_Equipment[i];
	}
}

void CPlayer::AddExp(int Exp)
{
	m_Exp += Exp;
	int LevelUpExp = CObjectManager::GetInst()->GetLevelUpExp(m_Level);
	if (m_Exp >= LevelUpExp)
	{
		if (m_Level == LEVEL_MAX)
		{
			m_Exp = LevelUpExp - 1;
		}
		else
		{
			m_Exp -= LevelUpExp;
			m_Level += 1;
		}
	}
}
bool CPlayer::Init()
{
	int _Menu;
	cout << "이름을 입력하세요 :" << endl;
	cin >> m_Name;
	while (true)
	{
		system("cls");
		cout << "1. 기사" << endl;
		cout << "2. 궁수" << endl;
		cout << "3. 마법사" << endl;
		cin >> _Menu;
		if (_Menu >= 1 || _Menu <= 3) break;
	}
	switch (_Menu)
	{
	case (int)Job::Knight:
		m_Job = Job::Knight;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case (int)Job::Archer:
		m_Job = Job::Archer;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case (int)Job::Magician:
		m_Job = Job::Magician;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	}

	 m_Exp = 0;
	 m_Gold = 1000;
	 m_Level = 1;
	
	return true;
}
void CPlayer::Output()
{
	cout << "이름 : " << m_Name << endl;
	cout << "직업" ;
	switch (m_Job)
	{
	case Job::Knight:
		cout << "기사" << endl;
		break;
	case Job::Archer:
		cout << "궁수" << endl;
		break;
	case Job::Magician:
		cout << "마법사" << endl;
		break;
	}
	cout << "공격력 : " << m_Attack;
	if (m_Equipment[Equip_Weapon])
		cout << " + " << m_Equipment[Equip_Weapon]->GetOption();
	cout << "방어력 : " << m_Attack;
	cout << endl;
	if (m_Equipment[Equip_Armor])
		cout << " + " << m_Equipment[Equip_Armor]->GetOption();
	cout << endl;
	cout << "체력 : " << m_HP << " / " << m_HPMax << endl;
	cout << "마력 : " << m_MP << " / " << m_MPMax << endl;
	cout << "경험치 : " << m_Exp << endl;
	cout << "골드 : " << m_Gold << endl;
	cout << "레벨 : " << m_Level << endl;
}

CItem* CPlayer::Equip(CItem* Item)
{
	Item_Type Type = Item->GetItemType();
	Equip_Type eType;
	switch (Type)
	{
	case IT_Weapon:
		eType = Equip_Weapon;
		break;
	case IT_Armor:
		eType = Equip_Armor;
		break;
	}
	CItem* EquipItem = m_Equipment[eType];
	m_Equipment[eType] = Item;
	return EquipItem;
}



bool CPlayer::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP < Damage)
	{
		m_HP = m_HPMax;
		return true;
	}
	return false;
}

void CPlayer::Death()
{
	m_Exp -= 0.1 * m_Exp;
	m_Gold -= 0.1 * m_Gold;
}
