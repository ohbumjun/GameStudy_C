#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

// 전략 패턴 -> 행위를 클래스로 캡슐화 하여
// 동적으로 행위를 자유롭게 바꿀 수 있게 해주는 패턴

// "전략" 에 해당하는 Class 들의 추상 Interface 를 정의하고
// 각각의 "Client" 가 해당 "전략"의 Class 들을 소유. 하게 하는 원리이다.

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
