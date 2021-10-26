#include <iostream>

using namespace std;

// Adaptee
class BT
{
public :
	static void Activate() { cout << "bt activate" << endl; }
	static void Deactivate() { cout << "bt deactivate" << endl; }
};

class Wifi
{
public:
	static void Activate() { cout << "Wifi activate" << endl; }
	static void Deactivate() { cout << "Wifi deactivate" << endl; }
};

// Target Interface
class NetWorkController
{
public:
	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
};


// Specific Classese
class WifiController : public NetWorkController
{
public :
	virtual void Activate() { Wifi::Activate(); }
	virtual void Deactivate() { Wifi::Deactivate(); }
};

class BTController : public NetWorkController
{
public:
	virtual void Activate() { BT::Activate(); }
	virtual void Deactivate() { BT::Deactivate(); }
};

int main()
{
	NetWorkController* ctrl = new BTController();
	ctrl->Activate();
	ctrl->Deactivate();

	ctrl = new WifiController();
	ctrl->Activate();
	ctrl->Deactivate();


	return 0;
}