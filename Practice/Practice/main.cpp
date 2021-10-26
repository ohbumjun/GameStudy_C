// ���Ͻ� ����
// �ƹ��� ����� ������ ����� ��ü(������)�� �����ϸ� �ذ��Ҽ� �ִ�.

#define _CRT_SECURE_NO_WARNINGS

//����
//	=>  ��������� �����ϴ� string Ŭ������ �����غ��ô�.
//	=> COW( Copy On Write ) ����� Ȱ���ؼ� String�� �ۼ��غ��ô�

#include<iostream>
#include<cstdlib>
#include<string>
#include<stdio.h>

using namespace std;

class String
{
	int* ref;
	char* buff;
public:
	String(const char* s)
	{
		buff = new char[strlen(s) + 1];
		strcpy(buff, s);
		ref = new int(1);
	}
	String(const String& s) : buff(s.buff), ref(s.ref)
	{
		++(*ref);
	}
	//[] ������ : ��ü�� �迭ó�� ���̰� �Ѵ�.
	char& operator[](int index)
	{
		cout << "operator []" << endl;
		return buff[index];
	}
};
int main()
{
	// ù��° ���� "����"������
	// �ι�° ���� "����"������  
	String s1 = "hello";
	String s2 = s1; // �޸𸮸� �� �����غ�����. ��������ڸ� ������

	char c = s1[0]; // �д� �ڵ�. �ڿ��� ��� ���� �Ǿ�� �Ѵ�.
	//cout << c << endl; // 'h'
	s1[0] = 'x'; // ���� �ڵ�. �ڿ��� �и��Ǿ�� �Ѵ�.
	// COW(Copy On Write)
	// cout << s1 << endl; // "xello"
	//cout << s2 << endl; // "hello"
}







