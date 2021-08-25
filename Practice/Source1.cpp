#include<iostream>

using namespace std;

class String
{
	int* ref;
	char* buff;
public :
	String(const char* c)
	{
		buff = new char[strlen(c) + 1];
		strcpy_s(buff, strlen(c) + 1, c);
		ref = new int(1);
		cout << "생성자" << endl;
	}
	// 복사 생성자 
	String(String& str) : ref(str.ref), buff(str.buff)
	{
		cout << "복사 생성자" << endl;
	}
	class CharProxy
	{
		String* s;
		int idx;
	public :
		CharProxy(String* str, int index) : s(str),idx(index)
		{
		}
		operator char()
		{
			cout << "change " << endl;
			return s->buff[idx];
		}
		CharProxy& operator = (const char& c)
		{
			cout << "charProxy operator" << endl;
			s->buff[idx] = c;
			return *this;
		}
	};
	CharProxy operator [] (int Index)
	{
		cout << "[] operator" << endl;
		return CharProxy(this, Index);
	}
	friend ostream& operator << (ostream& out, const String& str)
	{
		return out << str.buff << endl;
	}
};

int main()
{
	String s1 = "hello";
	String s2 = s1;
	cout << s1 << endl;
	char c = s1[0];
	s1[0] = 'x';
	cout << s1 << endl;

}