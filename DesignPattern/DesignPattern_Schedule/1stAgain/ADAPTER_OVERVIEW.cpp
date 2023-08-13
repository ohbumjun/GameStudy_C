/*
>> Differences
- Adapter pattern is about making
2 interfaces that arent't compatible
compatible

- Facade Pttern is, taking a bunch of
complex interactions and creating
a facade that you can use
instead of dealing with complex
objects and complext interactions
(that is, hide some complex logic)

- Proxy is a way of placing a
proxy between sth you want to call.
More accurately, you want to call sth
but you call the proxy instead
ex) security, caching etc.
(that is, intercepts a call and
thus controls access to the
underlying object )

- Decorator is a way of adding 
behaviour to some particular object
without actually opening that up 
and changing that object
(that is, adds behavior to 
some particular object)

----------------------------------------
>> Intent
ex) 110v, 220v adapter 
A standard ---> to ---> B standard
or
A interface ---> to ---> B interface,
which makes two compatible

that is, "Adapter" 
-> input : multiple type of inputs
-> output: single type of output 

Along above process, 
we don't want to change
underlying behaviour
ex) you just have adapter in between
ex) you are not adding,removing,editing
behaviour
ex) intent is to just simply
pass on the request 
ex) intent is to make incompatible
interface compatible

>> Definition
the adapter pattern converts a interface
of one class to another interface 
that client expects

the adapter lets classes work together
that couldn't otherwise because of
incompatible interface 

---------------------------------------
>> Diagram
class Client
{

}

// Client, "use" ITarget
// ex) "I" use "battery 200v charger" 
// request : "want to use battery 200v charger" 
class ITarget // interface 
{
    virtual void request()=0;
}

// concrete implementation of
// ITarget interface 
class Adapter : public ITarget
{
    Adaptee *adaptee;
public :
    Adapter(Adaptee* i_adt):adaptee(i_adt){}
    void request(){
        // need to delegate to Adaptee
        this.adaptee.specificRequest();        
    }
} 

// But, Adapter "delegates" to
// another class
// that follows completely different
// interface
// That is, "Adapter" 'has' Adaptee
// and 'use' Adaptee
class Adaptee 
{
    public :
        void specificRequest(){
            // ~
        }
}

// "Client" uses "Target"
// and "Target" has method "request"
// but, we want to use different
// request, which is "specialRequest"
// in "Adaptee" class
// so we cannot simply call "request"
// since "Adaptee" class does not have
// "request" method inside 
// so we let "request" method 
// in "Adapter" , invoke 
// "specificRequest" in "Adaptee" class 

----------------------------------------
>> code
int main()
{
    ITarget target = new Adapter(
        new Adaptee();
    )
    target.request();

}

정리
1) Client는 Target Interface만 본다
2) Adapter는 Target Interface를 따른다
3) Adapter는 Adaptee로 구성되어 있고
4) 모든 요청은 Adapter가
Adaptee로 Delegate 한다. 

https://anywayjhwa.tistory.com/12

*/

// example
#include <iostream>
#include <memory>

using namespace std;

// Target Interface
class IndianSocket
{
public:
    virtual void indianCharge(int) = 0;
} 

// different typeof charger : Indian,USA
// Adapteeclass USASocket
{
public:
    void usaCharge()
    {
        cout << "USA plug is charging" << endl;
    }
}

class GSocket
{
public:
    void gCharge()
    {
        cout << "Germany plug is charging" << endl;
    }
}

// This is a Adapter, use to charge with USASocket
// multiple input --> single output ...?
class SocketAdapter : public IndianSocket,
                      public USASocket,
                      public GSocket
{
public:
    // you are charing "indianCharge"
    // but actually it is calling "usaCharge"
    void indianCharge(int type)
    {
        switch (type)
        {
        case 1:
            usaCharge();
            break;
        case 2:
            gCharge();
            break;
        default:
            break;
        }
    }
}

int
main()
{
    // want to put "indianSocket" in "usaCharger"
    // now, you are creating "IndianSocket"
    // and, you are attaching "Adapter" to that
    unique_ptr<IndianSocket> socket = make_unique<SocketAdapter>();
    socket->indianCharge(1);
    socket->indianCharge(1);
    return 0;
}