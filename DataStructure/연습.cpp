#include <iostream>

using namespace std;

class Employee
{
private :
    int cost;
public :
    void GetPay()
    {
        cout << "Em" << endl;
    }
    void ShowInfo()
    {
        GetPay();
    } 
};

class PermanentWorker : public Employee
{
private :
    int cost;
public :
    void GetPay()
    {
        cout << "Per" << endl;
    }
};

int main()
{
	PermanentWorker wk;
	wk.ShowInfo();
}
