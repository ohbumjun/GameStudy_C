#include "Player.h"
#include "ObjectManager.h"
#include"Item.h"

using namespace std;

CPlayer::CPlayer() :
	m_Name{},
	m_Job(Job::None),
	m_Armor(0),
	m_Attack(0),
	m_HP(0),
	m_HPMax(0),
	m_MP(0),
	m_MPMax(0),
	m_Gold(0),
	m_Exp(0),
	m_Level(0)
{

}

CPlayer::~CPlayer()
{

}

bool CPlayer::Init()
{
	cout << "�̸��� �Է��ϼ��� : ";
	cin >> m_Name;

	int PlayerJob;
	
	while (true)
	{
		system("cls");
		cout << "1. ���" << endl;
		cout << "2. �ü�" << endl;
		cout << "3. ������" << endl;
		cout << "������ �����ϼ���";
		cin >> PlayerJob;

		if ((int)Job::None < PlayerJob && PlayerJob < (int)Job::End) break;
	}

	// --------------------------------------------------------------------

	m_Job = (Job)PlayerJob;

	switch (m_Job)
	{
		case Job::Knight:
			m_Attack = 200;
			m_Armor = 500;
			m_HP = 500;
			m_HPMax = 500;
			m_MP = 200;
			m_MPMax = 200;
			break;
		case Job::Archer:
			m_Attack = 200;
			m_Armor = 500;
			m_HP = 500;
			m_HPMax = 500;
			m_MP = 200;
			m_MPMax = 200;
			break;
		case Job::Magicion:
			m_Attack = 200;
			m_Armor = 500;
			m_HP = 500;
			m_HPMax = 500;
			m_MP = 200;
			m_MPMax = 200;
			break;
	}
	m_Level = 1;
	m_Exp = 0;
	m_Gold = 10000;
	return true;
}
CItem* CPlayer::Equip(CItem* Item)
{
	return nullptr;
}
void CPlayer::Output()
{
	cout << "�̸� : " << m_Name << endl;
	cout << "ü�� : " << m_HP << " / " << m_HPMax << endl;
	cout << "���� : " << m_MP << " / " << m_MPMax << endl;
	cout << "���ݷ� : " << m_Attack;
	if (m_Equipment[Equip_Weapon])
		cout << " + " << m_Equipment[Equip_Weapon]->GetOption();
	cout << endl;

	cout << "���� ���� : ";
	if (m_Equipment[Equip_Weapon])
		cout << " + " << m_Equipment[Equip_Weapon]->GetName();
	else cout << "���� " << endl;
	cout << endl;

	cout << "���� : " << m_Armor;
	if (m_Equipment[Equip_Armor])
		cout << " + " << m_Equipment[Equip_Armor]->GetOption() ;
	cout << endl;
	
	cout << "���� �� : ";
	if (m_Equipment[Equip_Armor])
		cout << " + " << m_Equipment[Equip_Armor]->GetName() ;
	else cout << "���� " << endl;
	cout << endl;

	cout << "����ġ : " << m_Exp << endl;
	cout << "���� : " << m_Level << endl;
	cout << "��� : " << m_Gold << endl;
}

void CPlayer::Death()
{
	cout << "�÷��̾ ����߽��ϴ�" << endl;
	m_Exp  -= 0.1f * m_Exp;
	m_Gold -= 0.1f * m_Gold;
}

void CPlayer::AddGold(int Gold)
{
	m_Gold += Gold;
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
			m_Level += 1;
			m_Exp -= LevelUpExp;
			switch (m_Job)
			{
				case Job::Knight:
					m_Attack += 200;
					m_Armor += 500;
					m_HP += 500;
					m_HPMax += 500;
					m_MP += 200;
					m_MPMax += 200;
					break;
				case Job::Archer:
					m_Attack += 200;
					m_Armor += 500;
					m_HP += 500;
					m_HPMax += 500;
					m_MP += 200;
					m_MPMax += 200;
					break;
				case Job::Magicion:
					m_Attack += 200;
					m_Armor += 500;
					m_HP += 500;
					m_HPMax += 500;
					m_MP += 200;
					m_MPMax += 200;
					break;
			}
		}
	}
}

bool CPlayer::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP < 0)
	{
		m_HP = m_HPMax;
		return true;
	}
	return false;
}