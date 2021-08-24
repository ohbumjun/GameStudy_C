#include<iostream>
#include<cstdlib>
#include<string>
#include<cstring>

using namespace std;

class String
{
	char* buff;
	int* ref;
	
public :
	// 대입 연산자
	String(const char* c){
		buff = new char[strlen(c) + 1];
		strcpy_s(buff, strlen(c) + 1, c);
		ref = new int(1);
	}
	// 복사 생성자
	String(String& s):buff(s.buff),ref(s.ref){}
	
	class CharProxy
	{
		String* str;
		int index;
	public :
		CharProxy(String *string,int i) : str(string), index(i){}
		operator char()
		{
			return str->buff[index];
		}
		CharProxy operator = (char s)
		{
			str->buff[index] = s;
			return *this;
		}
	};
	CharProxy operator [] (int index)
	{
		return CharProxy(this, index);
	}
	friend ostream& operator << (ostream& out, String& s)
	{
		return out << s.buff << endl;
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
	return 0;
}