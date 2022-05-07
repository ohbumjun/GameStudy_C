#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

// ���� ���� -> ������ Ŭ������ ĸ��ȭ �Ͽ�
// �������� ������ �����Ӱ� �ٲ� �� �ְ� ���ִ� ����

// "����" �� �ش��ϴ� Class ���� �߻� Interface �� �����ϰ�
// ������ "Client" �� �ش� "����"�� Class ���� ����. �ϰ� �ϴ� �����̴�.

struct IValidator
{
	virtual bool validate(string s, char c) = 0;
	virtual bool iscomplete(string s) { return true; }
};

struct LimitDigitValidator : public IValidator
{
	int value;

public :
	LimitDigitValidator(int n) : value(n)
	{

	}
	virtual bool validate(string s, char c) override
	{
		return s.size() < value && isdigit(c);
	}
	virtual bool iscomplete(string s)
	{
		return s.size() == value;
	}
};

struct LineEdit
{
	string data;
	IValidator* pValidator;
public :
	LineEdit() : pValidator(0){}
	void setValidator(IValidator* Validator) { pValidator = Validator; }
	string getData()
	{
		data.clear();

		while (1)
		{
			char c = _getch();

			if (pValidator == 0 || pValidator->iscomplete(data))
				break;
			if (pValidator == 0 || pValidator->validate(data, c))
			{
				data.push_back(c);
				cout << c;
			}
		}
		cout << endl;
		return data;
	}
};


int main()
{
	LineEdit edit;

	LimitDigitValidator v(5);

	edit.setValidator(&v);

	while (1)
	{
		string s = edit.getData();
		cout << "s : " << s << endl;
	}

	return 0;
}
