#include <iostream>

using namespace std;

// Adaptee
class BluetoothFW
{
public:
    static void ActivateBT()
    {
        cout << "BT Activated" << endl;
    }
    static void DeactivateBT()
    {
        cout << "BT Deactivated" << endl;
    }
};
class WifiFW
{
public:
    static void ActivateWifi()
    {
        cout << "Wifi Activated" << endl;
    }
    static void DeactivateWifi()
    {
        cout << "Wifi Deactivated" << endl;
    }
};

// Target Interface : 불루투스, wifi 키고 끄는 기능
class NetWorkController
{
public:
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
};

// Adapater
class BluetoothController
    : public NetWorkController
{
public:
    virtual void Activate()
    {
        cout << "Save Log Activate BT" << endl;
        BluetoothFW::ActivateBT();
    }
    virtual void Deactivate()
    {
        cout << "Save Log Dectivate BT" << endl;
        BluetoothFW::DeactivateBT();
    }
};

class WifiController
    : public NetWorkController
{
public:
    virtual void Activate()
    {
        cout << "Save Log Activate Wifi" << endl;
        WifiFW::ActivateWifi();
    }
    virtual void Deactivate()
    {
        cout << "Save Log Dectivate Wifi" << endl;
        WifiFW::DeactivateWifi();
    }
};

int main()
{
    NetWorkController *ctrl = new BluetoothController();
    ctrl->Activate();
    ctrl->Deactivate();

    delete ctrl;

    ctrl = new WifiController();
    ctrl->Activate();
    ctrl->Deactivate();

    return 0;
}