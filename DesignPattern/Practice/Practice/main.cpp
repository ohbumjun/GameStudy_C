#include<iostream>

using namespace std;

// Target Interface
struct IndianSocket
{
	virtual void indianCharge(int) = 0;
};

// Adaptee
struct UsaSocket
{
	void usaCharge() { cout << "usa charge" << endl; }
};

struct GSocket
{
	void gCharge() { cout << "g charge" << endl; }
};

// Adapter
struct SocketAdpater : 
	public IndianSocket,
	public GSocket,
	public UsaSocket
{
	virtual void indianCharge(int type) override
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
	unique_ptr<IndianSocket> Socket = make_unique<SocketAdpater>();
	Socket->indianCharge(1);
	Socket->indianCharge(2);

	return 0;
}