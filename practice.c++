#include <iostream>

using namespace std;

class Exception
{
public :
    void Show(){cout << "except" << endl;}
};

class Exception1 : public Exception
{
public :
    void Show(){cout << "except1" << endl;}
};

class Exception2 : public Exception
{
public :
    void Show(){cout << "except2" << endl;}
};

int main()
{
    try
    {
        throw Exception1();
    }
    catch(Exception& e)
    {
        e.Show();
    }
    
    return 0;
}