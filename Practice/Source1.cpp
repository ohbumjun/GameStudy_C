#include<iostream>

using namespace std;

class LightSwitch;
struct IState
{
	virtual void on(LightSwitch* ls)
	{
		cout << "Light already on" << endl;
	}
	virtual void off(LightSwitch* ls)
	{
		cout << "Light already off" << endl;
	}
};

class OnState : public IState
{
public :
	OnState() { cout << "Light Turned On" << endl; }
	virtual void off(LightSwitch* ls) override;
};

class OffState : public IState
{
public :
	OffState() { cout << "Light Turned Off" << endl; }
	virtual void on(LightSwitch* ls) override;
};

class LightSwitch
{
	IState* state;
public :
	LightSwitch()
	{
		state = new OffState();
	}
	void set_state(IState* st)
	{
		state = st;
	}
	void on() { state->on(this); }
	void off() { state->off(this); }
};

void OnState::off(LightSwitch* ls)
{
	cout << "Turning Light Off" << endl;
	ls->set_state(new OffState());
	delete this;
;}

void OffState::on(LightSwitch* ls)
{
	cout << "Turning Light On" << endl;
	ls->set_state(new OnState());
	delete this;
	;
}


int main()
{
	LightSwitch ls;
	ls.on();
	ls.off();
	ls.off();
	return 0;
}