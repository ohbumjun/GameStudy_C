#include<iostream>
#include<string>

using namespace std;

class String
{
	char* buff;
	int* ref;
public :
	String(const char* s)
	{
		buff = new char[strlen(s) + 1];
		strcpy_s(buff, strlen(s) + 1, s);
		ref = new int(1);
	}
	String(const String& s) :
		buff(s.buff),ref(s.ref){}
	char& operator [] (int index)
	{
		cout << "operator []" << endl;
		return buff[index];
	}
	friend ostream& operator << (ostream& out, const String& s)
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
	cout << s2 << endl;
}