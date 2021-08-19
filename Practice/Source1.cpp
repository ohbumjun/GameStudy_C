#include<iostream>
#include<stack>
#include<vector>

using namespace std;

// command
// encapsulate command
// make command as an object
// can pass it as parameter, even collection of commands can be in object
// action is encapsulated inside the command 

class Shape
{
public :
	virtual ~Shape(){}
public :
	virtual void Draw() = 0;
};

class Rect : public Shape
{
public :
	void Draw()
	{
		cout << "Draw Rect" << endl;
	}
};

class Circle : public Shape
{
public :
	void Draw()
	{
		cout << "Draw Circle" << endl;
	}
};

struct ICommand
{
	virtual void Execute() = 0;
	virtual bool CanUndo() { return false; }
	virtual void Undo(){}
	virtual ~ICommand(){}
};

class AddCommand : public ICommand
{
	vector<Shape*>& v;
public :
	AddCommand(vector<Shape*>&a) : v(a){}
	void Execute() override
	{
		v.push_back(CreateShape());
	}
	bool CanUndo() override { return true; }
	void Undo() override
	{
		Shape* p = v.back();
		v.pop_back();
		delete p;
	}
	virtual Shape* CreateShape() = 0;
};

class DrawCommand : public ICommand
{
	vector<Shape*>& v;
public:
	DrawCommand(vector<Shape*>& a) : v(a) {}
	void Execute() override
	{
		for (int i = 0; i < v.size(); i++)
			v[i]->Draw();
	}
	bool CanUndo() override { return true; }
	void Undo() override
	{
		system("cls");
	}
	virtual Shape* CreateShape() = 0;
};

class MacroCommand : public ICommand
{

};

int main()
{

}