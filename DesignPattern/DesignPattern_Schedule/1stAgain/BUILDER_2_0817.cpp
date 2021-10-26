// Why builder design pattern ?

// 복잡한 단계를 거쳐야 생성되는 객체의 구현을
// 서브 클래스에게 위임하는 것

// 즉, I want to build an ojbect and it is composed of
// complext objects step by step

#include <iostream>
#include <string>

using namespace std;

// Product
class Plane
{
    string m_Type;
    string m_Body;
    string m_Engine;

public:
    Plane(string planeType) : m_Type(planeType) {}
    void setEngine(string engine)
    {
        m_Engine = engine;
    }
    void setBody(string body)
    {
        m_Body = body;
    }
    string getEngine() { return m_Engine; }
    string getBody() { return m_Body; }
    void show()
    {
        cout << "type : " << m_Type << endl;
        cout << "Body : " << m_Body << endl;
        cout << "Engine : " << m_Engine << endl;
    }
};

// Plane Builder Absract Class
class PlaneBuilder
{
protected:
    Plane *m_Plane;

public:
    virtual void getPartsDone() = 0;
    virtual void buildBody() = 0;
    virtual void buildEngine() = 0;
    Plane *getPlane() { return m_Plane; }
};

// concrete builder class
class PropellerBuilder : public PlaneBuilder
{
public:
    void getPartsDone()
    {
        m_Plane = new Plane("Propeller Plane");
    }
    void buildEngine()
    {
        m_Plane->setEngine("Propeller Engine");
    }
    void buildBody()
    {
        m_Plane->setBody("Propeller Body");
    }
};

class JetBuilder : public PlaneBuilder
{
public:
    void getPartsDone()
    {
        m_Plane = new Plane("Jet Plane");
    }
    void buildEngine()
    {
        m_Plane->setEngine("Jet Engine");
    }
    void buildBody()
    {
        m_Plane->setBody("Jet Body");
    }
};

// 구축 공정
class Director
{
    PlaneBuilder *builder;

public:
    void setBuilder(PlaneBuilder *b)
    {
        builder = b;
    }
    Plane *createPlane()
    {
        builder->getPartsDone();
        builder->buildEngine();
        builder->buildBody();
        return builder->getPlane();
    }
};

int main()
{
    Director dir;
    JetBuilder jb;
    PropellerBuilder pb;

    dir.setBuilder(&jb);
    Plane *jet = dir.createPlane();
    jet->show();

    dir.setBuilder(&pb);
    Plane *propeller = dir.createPlane();
    propeller->show();

    return 0;
}
