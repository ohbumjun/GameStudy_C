// String

#include <iostream>
#include <cstring>

using namespace std;

class String
{
private :
    int len;
    char * str;
public :
    // 기본 생성자
    String();
    String(const char* s);
    // 복사 생성자 
    String(const String& s);
    // 소멸자
    ~String();
    // 연산자 오버로딩
    String& operator = (const String& s);
    String& operator += (const String& s);
    bool operator == (const String& s);
    String& operator + (const String& s);

    friend ostream& operator << (ostream& os, const String& s);
    friend istream& operator >> (istream& is, String& s);
};

Stirng::String()
{
    len = 0;
    str = NULL;
}
String(const char* s)
{
    len = strlen(s) + 1;
    str = new char[len];
    strcpy(str, s);
}

// 복사 생성자 
String(const String& s)
{
    len = s.len;
    str = new char[len];
    strcpy(str, s.str);
}

// 소멸자
~String()
{
    if(str)
        delete [] str;
}

// 연산자 오버로딩
String& operator = (const String& s)
{
    if (str)
        delete [] str;
    len = s.len;
    str = new char[len];
    strcpy(str, s.str);
    return *this;
}
String& operator += (const String& s)
{
    len += (s.len - 1);
    char* temp = new char[len];
    strcpy(temp, str);
    strcat(temp, s.str);
    
    if (str)
        delete [] str;
    str = temp;
    return *this;
}
bool operator == (const String& s)
{
    // 같으면 0 반환
    return strcmp(str, s.str) ? false : true;
}
String& operator + (const String& s)
{
    int t_len = len + (s.len - 1);
    char* temp = new char[t_len];
    strcpy(temp, str);
    strcat(temp, s);

    String tempStr(temp);
    delete [] temp;
    return tempStr;
}

friend ostream& operator << (ostream& os, const String& s)
{
    os << s.str;
    return os;
}
friend istream& operator >> (istream& is, String& s)
{
    char str[100];
    is >> str;
    s = String(str);
    return is;
}
