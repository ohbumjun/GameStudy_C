#include<iostream>

using namespace std;

struct IState
{
	virtual void run() = 0;
	virtual void attack() = 0;
};

class NormalState : public IState
{
	void run() { cout << "run" << endl; }
	void attack() { cout << "attack" << endl; }
};

class FastState : public IState
{
	void run() { cout << "fast run" << endl; }
	void attack() { cout << "fast attack" << endl; }
};

class Character
{
	IState* state;
public :
	void setState(IState* st) { state = st; }
	void run() { state->run(); }
	void attack() { state->attack(); }
};

int main()
{
	Character character;
	NormalState ns;
	FastState fs;
	character.setState(&ns);
	character.attack();
	character.run();
	character.setState(&fs);
	character.attack();
	character.run();
	
	return 0;
}