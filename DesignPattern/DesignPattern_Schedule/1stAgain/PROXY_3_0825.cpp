#include <iostream>

// Proxy Pattern의 사례
// Shared_ptr, Unique_ptr

using namespace std;

class Subject
{
public:
    virtual void request() = 0;
    virtual ~Subject() {}
};

class RealSubject : public Subject
{
public:
    void request()
    {
        cout << "RealSubject.request()" << endl;
    }
};

class Proxy : public Subject
{
private:
    Subject *realSubject;

public:
    Proxy() : realSubject(new RealSubject())
    {
    }
    ~Proxy() { delete realSubject; }
    void request()
    {
        realSubject->request();
    }
}

int
main()
{
    Proxy p;
    p.request();
}