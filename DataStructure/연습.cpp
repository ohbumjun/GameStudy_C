// �����Լ��� ȣ�� ���迡�� ���� Ư��, 
// ��, �ش� �Լ� ȣ�� ��, �Լ��� �����Լ���� �������� �ڷ����� ������� ȣ������ �������� �ʰ�
// ������ ������ ������ ����Ű�� ��ü�� �����Ͽ�, ȣ���� ����� �����ϴ� ����

// ������ ������ , ����� �ٸ� Ư��
// �������̶� �ϳ��� Ÿ�Կ� ���� ��ü�� ������ �� �ִ� ����
#include<iostream>

using namespace std;

class First
{
public :
    virtual void func()
	{
		cout << "First" << endl;
	}
};

class Second : public First
{
public :
    void func()
    {
    	cout << "Second" << endl;
	}
};

class Third : public Second 
{
public :
    void func()
    {
    	cout << "Third" << endl;
	}
};

int main()
{
    First* fptr = new First;
    fptr->func();
    delete fptr;
    
    fptr = new Second;
    fptr->func();
    delete fptr;

    fptr = new Third;
    fptr->func();
}
