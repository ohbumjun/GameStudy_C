#include<iostream>
#include<vector>
#include<stdint.h>

using namespace std;

class BankAccount
{
	int32_t m_balance{ 0 };
	uint32_t m_current{ 0 };
	
	struct Memento
	{
		int32_t m_balance;
	public :
		Memento(int32_t b):m_balance(b){}
	};

	vector<shared_ptr<const Memento>> m_change;

public :
	BankAccount(int32_t m)
	{
		m_change.emplace_back(make_shared<const Memento>(m));
	}
	const shared_ptr<const Memento> deposit(int32_t m)
	{
		m_balance += m;
		m_change.emplace_back(make_shared<const Memento>(m));
		return m_change[++m_current];
	}
	void restore(const Memento& m)
	{
		m_balance = m.m_balance;
		m_change.emplace_back(m);
		m_current = m_change.size() - 1;
	}
	const shared_ptr<const Memento> undo()
	{
		if (m_current > 0)
		{
			m_balance = m_change[--m_current]->m_balance;
			return m_change[m_current];
		}
		cout << "*** Attempt to run off the end!! ***" << endl;
		return {};
	}
	const shared_ptr<const Memento> redo()
	{
		if (m_current + 1 < m_change.size())
		{
			m_balance = m_change[++m_current]->m_balance;
			return m_change[m_current];
		}
		cout << "*** Attempt to run off the end!! ***" << endl;
		return {};
	}
	friend ostream& operator << (ostream& os,
		const BankAccount& bc)
	{
		return os << "balance : " << bc.m_balance;
	}
};



int main()
{
	BankAccount ac{ 10 };
	ac.deposit(10);
	cout << ac << endl;
	return 0;
}