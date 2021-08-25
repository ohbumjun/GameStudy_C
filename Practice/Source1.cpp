#include<iostream>

using namespace std;

class LightSwitch;

struct IState
{
	virtual void on(LightSwitch* ls)
	{
		cout << "Light already turned on" << endl;
	}
	virtual void off(LightSwitch* ls)
	{
		cout << "Light already turned off" << endl;
	}
};

struct OnState : public IState
{
	OnState()
	{
		cout << "Light Turned On" << endl;
	}
	void off(LightSwitch* ls);
};

struct OffState : public IState
{
	OffState()
	{
		cout << "Light Turned Off" << endl;
	}
	void on(LightSwitch* ls);
};

class LightSwitch
{
	IState* state;
public :
	LightSwitch() { state = new OffState(); }
public :
	void setState(IState* st) { state = st; }
	void on() { state->on(this); }
	void off() { state->off(this); }
};

void OnState::off(LightSwitch* ls)
{
	cout << "Turning Light off" << endl;
	ls->setState(new OffState());
	delete this;
}

void OffState::on(LightSwitch* ls)
{
	cout << "Turning Light On" << endl;
	ls->setState(new OnState());
	delete this;
}

int main()
{
	LightSwitch ls;
	ls.on();
	ls.off();
	ls.on();
	ls.on();
}




