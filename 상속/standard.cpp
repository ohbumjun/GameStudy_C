#include<iostream>

/*
��� : Ŭ������ �θ�� �ڽ��� ���踦 ������ �� �ִ�.

�ڽ� Ŭ������ �θ�Ŭ������ ������� Ȥ��
����Լ��� �����޾� ����� �� �ְ� �ȴ�.

��Ӱ��迡�� �����ڿ� �Ҹ����� ȣ����� 
������ : �θ�-->�ڽ�
�Ҹ��� : �ڽ�-->�θ�

protected :
Ŭ������ �ܺο�����, ������ �Ұ����ϰ�
Ŭ������ ���� Ȥ�� �ڽ� Ŭ������ ���ο�����
������ �����ϴ�

���� ����� ����ϴ� ������, 
Ŭ������ ���뼺�� �ϳ��� ������ �ȴ�.

�θ�Ŭ������ ���������� ������ �� ��������� �����ϰ�
�������� �������� ����� ����Լ��� ������� �Ŀ�
�������� ����鸸 �ڽ�Ŭ������ ����
���ݾ� ���� �ϳ��� ��ü�� ���� �� ����Ѵ�.

������ :
��� ���迡 �ִ� Ŭ�������� ���� ����ȯ�� �����ϴ�.
�پ��� �θ�� �ڽ� Ÿ�������� ����ȯ�� �������ֱ� ������
���� �θ� ��ӹް� �ִ� �ڽ�Ŭ������
�ش� �θ�Ÿ������ ��� �ڽ� Ŭ���� Ÿ���� ��������
�θ�Ÿ������ ������ ���������� �Ǵ� ���̴�.

��ĳ����   : �ڽ� -> �θ� Ÿ��
�ٿ�ĳ���� : �θ� -> �ڽ�

�����Լ� : �Լ� �տ� virtual�� �ٿ���
�����Լ��� ������� �� �ִ�.

�����Լ��� ������ �ִ� Ŭ������ �̿��ؼ�
��ü�� �����ϸ� ,
�����Լ� ���̺��� �ּҸ� ������ �ִ� 
������ ������ ���������� ������ �ȴ�.

�����Լ� ���̺��� �ش� Ŭ������ ������ �ִ�
�����Լ��� �Լ��ּҸ� �����ϰ� �ִ� 
�迭�̶�� �� �� �ִ�.

Ŭ������ �̿��ؼ� ��ü�� �����ϸ�, 
���ο� __vfptr ������ ������ ���������
�� ������ �����Լ� ���̺� �迭�� �޸� ���� �ּҰ�
���� �ȴ�.

�����Լ��� �ڽ�Ŭ�������� �ش� �Լ��� 
�������� �� �ִ�.
�̸� �Լ� �������̵� �̶�� �Ѵ�.
�Լ��� ������ �Ҷ��� ��ȯŸ��, ���ڰ� ��� �����ؾ� �Ѵ�.
�Լ� �̸��� �翬�� �����ؾ� �Ѵ�.

�Լ� �������̱� ������, �� �״�� �ش� �Լ���
�ڽ� Ŭ�������� �ٽ� ����, 
�ڽ� Ŭ������ �Լ��� ȣ��� �� �ֵ���
������ִ� ����� ���̴�. 

�Ҹ��ڴ� ��� Ŭ������ ����ǰ� �̸��� �ٸ� ��
�Ҹ����̹Ƿ�, 

�θ� Ŭ������ �Ҹ��ڿ� virtual�� �ٿ��ָ�
�ڽ� Ŭ������ �ڵ����� �����ǵ� �Ҹ��ڷ� �ν��� �ǰ� �Ͽ�
�ش� �ڽ� Ŭ������ �Ҹ��Ų��.
�翬�� ����, �ڽ� Ŭ�������� �Ҹ�ǰ� ����, 
�θ� Ŭ������ �Ҹ�ȴ�.

�Ϲ� ����Լ��� virtual�� �ٿ���
�����Լ��� ������ָ�, �ڽ� Ŭ������ �����Ǹ� �ص��ǰ�
���ص� �ȴ�. 
���� CChild Ŭ������ �����ǰ� �Ǿ� �ְ�
CChild1 Ŭ������ �����ǰ� �ȵǾ� ���� ��� ,

�����Լ� ���̺� ����Ǵ� �Լ��� �ּ��� ��� 
CChild Ŭ������ �̿��ؼ�
������ ��ü�� CChild Ŭ������ �����ǵ� �Լ���
�ּҰ� ���� �ǰ�,

CChild1 Ŭ������ �̿��ؼ�, ������ ��ü��
�����ǰ� �ȵǾ� �����Ƿ�, CParent Ŭ������ 
���ǵ� �Լ��� �ּҰ� ���� �Ǵ� ���̴�.

�׷��� �����Լ��� ȣ���� �ϰ� �Ǹ�, 
���� �����Լ������� �Ǵ��ϰ�
�����Լ����, �����Լ� ���̺���
���� �Լ��� �ּҸ� ã�ƿ� �Ŀ�, �ش� �Լ��� �ּҸ�
�̿���, �Լ�ȣ���� �����ϰ� �ǹǷ� 

CChild Ŭ������ �̿��ؼ� ������ ��ü��
�����Լ����̺� CChildŬ������ �����ǵ� �Լ��� �ּҰ�
�� �����Ƿ�, �����ǵ� �Լ��� ȣ�����ְ�

CChild1 Ŭ������ �̿��ؼ� ������ ��ü��
�����Լ����̺� CParent Ŭ������ ����� �Լ���
�ּҰ� �� �����Ƿ�, �ش� �Լ��� ȣ���ϰ� �Ǵ� ���̴�.

*/
using namespace std;

class CParent
{
public :
	CParent()
	{
		cout << "CParent ������" << endl;
	}
	virtual ~CParent()
	{
		cout << "CParent �Ҹ���" << endl;
	}
public :
	int m_Number1;
private :
	int m_Number2;
protected :
	int m_Number3;
// �θ��� private ������, public ����Լ��� �ٲ��ֱ�
public :
	void SetNumber(int Num2)
	{
		m_Number2 = Num2;
	}
public :
	virtual void Output()
	{
		cout << "CParent Output" << endl;
	}
};

class CParent1
{
public :
	CParent1()
	{
		cout << "CParent1 ������" << endl;
	}
	~CParent1()
	{
		cout << "CParent1 �Ҹ���" << endl;
	}
public :
	int m_Number3;
};

// ���� ���
// ������ �� : �ߺ��Ǵ� ������� �� ����Լ�
class CChild2 : public CParent, CParent1
{
public:
	CChild2()
	{
		cout << "CChild2 ������" << endl;
	}
	~CChild2()
	{
		cout << "CChild2 �Ҹ���" << endl;
	}
};

class CChild : public CParent
{
public :
	CChild()
	{
		cout << "CChild ������" << endl;
		// �θ��� ��������� ������ �� �ִ�
		m_Number1 = 100;
		// m_Number2 = 300;
		m_Number3 = 500;
		m_NumberPointer = new int;
	}
	~CChild()
	{
		cout << "CChild �Ҹ���" << endl;
		delete m_NumberPointer;
	}
protected :
	int* m_NumberPointer;
public :
	// �Լ� �������̵�
	void Output()
	{
		// CParent::Output();
		cout << "CChild Output" << endl;
	}
	void ChildOutput()
	{
		cout << "CChild���� �ִ� Output" << endl;
	}
};

class CChild1 : private CParent
{
public :
	CChild1()
	{
		cout << "CChild1 ������" << endl;
		m_Number1 = 100;
		m_Number1 = 300;
	}
	~CChild1()
	{
		cout << "CChild1 �Ҹ���" << endl;
	}
	void ChildOutput()
	{
		cout << "CChild1 ���� �ִ� Output" << endl;
	}
};

class CChildChild : public CChild
{
public :
	CChildChild()
	{
		cout << "CChildChild ������" << endl;
		// �Ҿƹ������� ���� ����
		m_Number1 = 100;
		m_Number3 = 300;
	}
};

class CChildChild1 : public CChild1
{
public :
	CChildChild1()
	{
		cout << "CChildChild1 ������" << endl;
		// CChild1�� private���� ��ӹ޾� ������
		// ��� public���� CChild1�� ��ӹ޴���
		// CChild1 �� �θ��� CParent�� public, protected
		// ������ ������ �� ����. 
	}
	~CChildChild1()
	{
		cout << "CChildChild1 �Ҹ��� " << endl;
	}
};


// ��������� ���� ���, ũ�⸦ ���ϸ� 1����Ʈ�� ���´�
// ����ü�� Ŭ������ �ش� Ÿ���� �̿��Ͽ� ������
// ������ �� �ֱ� ������ 
// ex) Test test1;
// �ּ� ����Ʈ�� 1����Ʈ�� ���� ���� ������
// �����ϰ� ���ش�.
struct Test
{
};

class CTestParent
{
public :
	CTestParent(){}
	virtual ~CTestParent()
	{

	}
	virtual void Output(){}
	virtual void Output1(){}
	virtual void Output2(){}
public :
	int m_Test;
	int m_Test1;
	int m_Test2;
	int m_Test3;
};

class CTestChild : public CTestParent
{
public :
	CTestChild(){}
	~CTestChild(){}
};

int main()
{
	// 20 ? : 3�� int ������� + 1�� CChild int* �������  + 1�� �����Լ����̺� �ּ� ���� ������ ����(������)
	cout << sizeof(CChild) << endl;

	// ��������� �ϳ��� ���� Ŭ������
	// �ּ����� ������ 1byte�� �Ҵ�ȴ�
	// size���� ���"�Լ�"�� ���翩�δ� ������ ��ġ�� �ʴ´�
	// �ֳ��ϸ� "����Լ�"�� ��� , 
	// �ڵ念���� �ö󰡴� ������.
	// ��ü�� ������ ��, �޸𸮰� ������ ���� �ƴϱ� �����̴�
	cout << sizeof(Test) << endl;

	// 20 : int�� ����Լ� 4�� + 1�� �����Լ� ���̺� �ּ� ���� �迭 
	cout << sizeof(CTestChild) << endl;

	CTestChild testchild;

	// CChildŬ������ �����Ҵ��ϰ�
	// �ش� �޸� �ּҸ� CParent ������ Ÿ�Կ� ����
	// ���� ��Ӱ��迡 �ִ� Ŭ�����̱� ������
	// �̷��� ����ȯ�� ������ ���̴�
	CParent* Child  = new CChild;
	CParent* Child2 = new CChild2;
	CParent* ChildChild2 = new CChildChild;

	// virtual �Լ� --> Child Ŭ������ ���ǵ� �Լ� ȣ��
	Child->Output();
	// Parent�� ���ǵ� �Լ��� �ǵ������� ȣ�� 
	Child->CParent::Output();


	// Child�� �ٺ������� CParent* Ÿ���� �����̴�
	// �ٿ�ĳ������ �� ����, ���� �����ؼ� ���
	// �ֳ��ϸ�, Child�� CChild Ÿ�� Ŭ������ ��ü�� ������ ���̴�.
	// �׷��� ���� �̰��� CChild1 Ÿ������ ����ȯ�� �ϸ�
	// CChild���� --> CParent* ����ȯ -> CChild1 ����ȯ
	// �� ��ü�� �ٺ������� CChild��ü�� ������ ���̹Ƿ�
	// CChild1 Ÿ������ ����ȯ �Ͽ� ����� ��
	// ���� ������ �߻���ų �� �ִ�.

	// �׷��Ƿ� �ٿ�ĳ������ �� ����
	// ���� �ſ� �ſ� �����ؾ� �Ѵ�
	cout << endl;
	((CChild1*)Child)->ChildOutput();

	// CChild2�� ���, �Լ� ������ x
	// CParent�� Output �����Լ� ȣ��
	cout << endl;
	Child2->Output();

	// CChild�Ҹ��� --> �θ� �Ҹ���
	delete Child;
	// CChild2�Ҹ��� --> CParent,CParent1 �Ҹ���
	delete Child2;
	// ChildChild -> CChild -> CParent;
	delete ChildChild2;

	return 0;
}

