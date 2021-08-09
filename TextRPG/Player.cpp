#include "Player.h"
#include "Item.h"

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
}
bool CPlayer::Init()
{
	int _Menu;
	cout << "�̸��� �Է��ϼ��� :" << endl;
	cin >> m_Name;
	while (true)
	{
		system("cls");
		cout << "1. ���" << endl;
		cout << "2. �ü�" << endl;
		cout << "3. ������" << endl;
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
	cout << "�̸� : " << m_Name << endl;
	cout << "����" ;
	switch (m_Job)
	{
	case Job::Knight:
		cout << "���" << endl;
		break;
	case Job::Archer:
		cout << "�ü�" << endl;
		break;
	case Job::Magician:
		cout << "������" << endl;
		break;
	}
	cout << "���ݷ� : " << m_Attack;
	if (m_Equipment[Equip_Weapon])
		cout << " + " << m_Equipment[Equip_Weapon]->GetOption();
	cout << "���� : " << m_Attack;
	cout << endl;
	if (m_Equipment[Equip_Armor])
		cout << " + " << m_Equipment[Equip_Armor]->GetOption();
	cout << endl;
	cout << "ü�� : " << m_HP << " / " << m_HPMax << endl;
	cout << "���� : " << m_MP << " / " << m_MPMax << endl;
	cout << "����ġ : " << m_Exp << endl;
	cout << "��� : " << m_Gold << endl;
	cout << "���� : " << m_Level << endl;
}

CItem* CPlayer::Equip(Equip_Type Type)
{
	return nullptr;
}

bool CPlayer::Damage(int Damage)
{
	return false;
}
