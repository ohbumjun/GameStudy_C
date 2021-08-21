// �޸���
// ��ü�� ���¸� ���������� �����ߴٰ�
// ������ ���� ���·� ���������ϰ� �ϴ� ����

// Undo�� ������ ����
// Command : ������ ��Ҹ� ����
// Memento : �������� ����

#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Machine
{
	int stateA;
	int stateB;
	class Memento
	{
		friend class Machine;
		int stateA;
		int stateB;
	public :
		Memento(int a, int b) :stateA(a), stateB(b){}
	};
	vector<Memento*> back_up;

public :
	int CreateMemento()
	{
		Memento* m = new Memento(stateA, stateB);
		back_up.push_back(m);
		return back_up.size() - 1;
	}
	void SetState(int a, int b)
	{
		stateA = a;
		stateB = b;
	}
	void RestoreMemento(int token)
	{
		stateA = back_up[token]->stateA;
		stateB = back_up[token]->stateB;
	}
};

int main()
{
	Machine mc;
	mc.SetState(10, 20);
	int token = mc.CreateMemento();
	mc.SetState(40, 50);
	mc.RestoreMemento(token);
	return 0;
}