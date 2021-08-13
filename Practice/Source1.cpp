#include<iostream>
#include<memory>

using namespace std;

class IndianSocket
{
public :
	virtual void indianCharge(int type) = 0;
};

class GSocket
{
public :
	void gCharge()
	{
		cout << "g Charge" << endl;
	}
};

class USASocket
{
public :
	void usaCharge()
	{
		cout << "usa Charge" << endl;
	}
};

class SocketAdapter :
	public IndianSocket,
	public USASocket,
	public GSocket
{
public :
	void indianCharge(const int type)
	{
		switch (type)
		{
		case 1 :
			usaCharge();
			break;
		case 2 :
			gCharge();
			break;
		default:
			break;
		}
	}
};

int main()
{
	unique_ptr<IndianSocket>socket = make_unique<SocketAdapter>();
	socket->indianCharge(1);
	socket->indianCharge(2);
}