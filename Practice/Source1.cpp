#include<iostream>
#include<string>
#include<list>

using namespace std;

class IVisitor;
struct IElement
{
	virtual void accept(IVisitor* visitor) = 0;
};

class Wheel : public IElement
{
private :
	const char* m_Name;
public :
	Wheel(const char* name):m_Name(name){}
	const char* getName() { return m_Name; }
	void accept(IVisitor* visitor)override;
};

class Body : public IElement
{
	void accept(IVisitor* visitor) override;
};

class Engine : public IElement
{
	void accept(IVisitor* visitor) override;
};

class Car : public IElement
{
private :
	list<IElement*> m_List;
public :
	void push_back(IElement* e) { m_List.push_back(e); }
	void accept(IVisitor* visitor) override;
};

struct IVisitor
{
	virtual void visit(Wheel* acceptor) = 0;
	virtual void visit(Body* acceptor) = 0;
	virtual void visit(Engine* acceptor) = 0;
	virtual void visit(Car* acceptor) = 0;
};

// 차를 시동거는 visitor
class CarStartVisitor : public IVisitor
{
	virtual void visit(Wheel* acceptor) { cout << "kicking " << acceptor->getName() << endl; }
	virtual void visit(Body* acceptor) { cout << "Moving Body" << endl; }
	virtual void visit(Engine* acceptor) { cout << "Starting Engine" << endl; }
	virtual void visit(Car* acceptor) { cout << "Starting Car" << endl; }
};

// 차의 내용을 출력하는 visitor
class CarPrintVisitor : public IVisitor
{
	virtual void visit(Wheel* acceptor) { cout << "printing " << acceptor->getName() << endl; }
	virtual void visit(Body* acceptor) { cout << "priting Body" << endl; }
	virtual void visit(Engine* acceptor) { cout << "printing Engine" << endl; }
	virtual void visit(Car* acceptor) { cout << "printing Car" << endl; }
};

void Wheel::accept(IVisitor* visitor) { visitor->visit(this); }
void Body::accept(IVisitor* visitor) { visitor->visit(this); }
void Engine::accept(IVisitor* visitor) { visitor->visit(this); }
void Car::accept(IVisitor* visitor)
{
	auto iter = m_List.begin();
	auto iterEnd = m_List.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->accept(visitor);
	}
	visitor->visit(this);
}


int main()
{
	// 부품 준비
	Wheel m_Wheel[4] = { "front left","front right","back left","back right"};
	Body m_Body;
	Engine m_Engine;
	Car m_Car;

	m_Car.push_back(&m_Wheel[0]);
	m_Car.push_back(&m_Wheel[1]);
	m_Car.push_back(&m_Wheel[2]);
	m_Car.push_back(&m_Wheel[3]);
	m_Car.push_back(&m_Body);
	m_Car.push_back(&m_Engine);

	CarPrintVisitor pv;
	CarStartVisitor sv;

	m_Car.accept(&pv);
	m_Car.accept(&sv);

	return 0;
}