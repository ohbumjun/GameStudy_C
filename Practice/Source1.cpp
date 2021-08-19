#include<iostream>
#include<vector>
#include<stack>

using namespace std;

class Shape
{
public :
	Shape(){}
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
	virtual void Undo() = 0;
	virtual ~ICommand(){}
};

class AddCommand : public ICommand
{
	vector<Shape*>& v;
public :
	AddCommand(vector<Shape*>&a) :v(a){}
	void Execute()
	{
		v.push_back(CreateShape());
	}
	bool CanUndo() { return true; }
	void Undo()
	{
		Shape* p = v.back();
		v.pop_back();
		delete p;
	}
	virtual Shape* CreateShape() = 0;
};

class AddRectCommand : public AddCommand
{
public :
	AddRectCommand(vector<Shape*>&a) : AddCommand(a){}
	virtual Shape* CreateShape() override
	{
		return new Rect;
	}
};

class AddCircleCommand : public AddCommand
{
public :
	AddCircleCommand(vector<Shape*>&a):AddCommand(a){}
	virtual Shape* CreateShape() override
	{
		return new Circle;
	}
};

class DrawCommand : public ICommand
{
	vector<Shape*>& v;
public :
	DrawCommand(vector<Shape*>&a):v(a){}
	void Execute()
	{
		for (int i = 0; i < v.size(); i++)
			v[i]->Draw();
	}
	bool CanUndo() { return true; }
	void Undo()
	{
		system("cls");
	}
};

// 여러개 명령을 묶어서 사용하는
// 매크로 명령
class MacroCommand : public ICommand
{
public :
	// Composite 패턴
	vector<ICommand*> v;
public :
	void AddCommand(ICommand* p) { v.push_back(p); }
	void Execute()
	{
		for (int i = 0; i < v.size(); i++)
			v[i]->Execute();
	}
	void Undo(){}
};

int main()
{
	vector<Shape*> v;
	stack<ICommand*> cmd_stack;

	MacroCommand* p1 = new MacroCommand;
	p1->AddCommand(new AddRectCommand(v));
	p1->AddCommand(new DrawCommand(v));
	p1->Execute();

	MacroCommand* p2 = new MacroCommand;
	p2->AddCommand(new AddCircleCommand(v));
	p2->AddCommand(new DrawCommand(v));
	p2->Execute();

	return 0;
}