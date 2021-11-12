/*
>> Intent -----------------------------------------
- solve a problem that are access related
- it adds additional behavior with the intent of
controlling access to the underlying object 
- it adds additional behavior just before
you make an call to it 

>> Definition --------------------------------------
--> proxy pattern provides a surrogate or placeholder 
--> for another object
--> to control access to it

you have an object that you want to get
access to 
ex) you want to invoke methods on it 

but instead of being allowed to interact with that thing
you interact with proxy who interact with that thing

introducing another level of indirection 

>> Kinds ------------------------------------------
1) Remote : used when you want to access 
resource that is remote
ex) remote ? == different server, different namespace 
interaction with remote resource could be
wrapped in proxy 
- your proxy is responsible for interacting with
remote resource & giving you back the things you need 

2) Virtual
- controls access to resource 
that is expensive to create
ex) cashing 

3) Protection
ex) access management 
-- you are not sure whether particular user
has an access to particular resource 
-- proxy makes sure if you are not allowed access,
you cannot access in the very first place 


string bookString = "..........~ huge string...."

class BookParser
{
} 

BookParser bp = new BookParser(bookString)
bp.numberOfPages()
bp.numberOfChapters()

// BookParser
// would immediately parse "bookString" 
// and give you and method
// numberOfPages(), numberOfChapters()
// but parsing hugh "bookString" requires a lot of work

// What is problem ? : performance problem
// solution ? 
// --> make proxy in between whoever calls "BookParser"
// --> or whoever use "numberOfPages()"... methods 

1) contstruct : expensive 
2) use numberOfPages() methods

but "numberOfPages()" methods is not always used.
then, what we can do is , 
constructing, which is expensive operation, 
should only happen, only if you actually
want to call "numberOfPages()" methods 

Client -- Proxy -- BookParser
- Client interact with Proxy &&
Client instantiate Proxy
- Proxy interact with BookParser &&
Proxy "doesn't" instantiate Proxy 

why ? Proxy says, "hey, Client, I don't know
whether you will call the methods that requires
me to parse this book. So I will wait until you tell
me that you want to parse the book"

that is, we are "deferring" the instantiation
of BookParser 

If, Client wants to parse the book , 
Then, Proxy 
1) instantiate "BookParser" : expensive
2) call methods in "BookParser" : cheap
3) send result back to Client : cheap

Of course, Proxy should not instantiate BookParser
every time it wants to parse the book.
Proxy should cache "BookParser" , and maintain the
reference to that 

>> UML ---------------------------------------------
interface ISubject{
    virtual void request()
}
// below is "BookParser" that we talked above 
class RealSubject : public ISubject {
    void request(){

    }
}
// Proxy behaves as a "Subject"
class Proxy : public ISubject {
    // Proxy has "ConcreteSubject"
    // controls "access" to "ConcreteSubject"
    // ex) whenever you request for the 1st time,
    // ex) you delegate the request to "RealSubject"
    // ex) any call after the 1st call, 
    // ex) you will just get value that "Proxy" has 
    // ex) and it won't make actual call to "Proxy"
    ConcreteSubject *csj; 
    void request(){

    }
}

// 정리 ----------------------------------------------
Both the Proxy and the RealSubject implement the Subject interface. 
This allows any client to treat the Proxy just like the RealSubject.

The RealSubject is usually the object that does most of the real work; the Proxy controls access to it.

The control may be needed if the Subject is running on a remote machine, 
if the Subject is expensive to create in some way or if access to the subject needs to be protected in some way.

The Proxy often instantiates or handle the creation of the RealSubject.
The Proxy keeps a reference (pointer) to the Subject, 
so it can forward requests to the Subject when necessary.
In some cases, the Proxy may be responsible for creating and destroying the RealSubject. 
Clients interact with the RealSubject through the Proxy.

// --------------------------------------------------
class IBookParser{
    virtual int getNumOfPages()=0;
}
class BookParser : public IBookParser {
    BookParser(string book){
        // expensive parsing
    }
    int getNumOfPages(){   
    }
}

// Proxy
class LazyBookParserProxy : public IBookParser{
// instance variable 은 우선 막는다
private :
    BookParser parser = null : 
    string book = null;
public :
    LazyBookParserProxy(string book){
        this.book = book
    }
    int getNumOfPages()
    {
        // want to delegate to bookparser
        if(parser==null) this.parser = new BookParser(string r
        representing the book)
        return this.parser.getNumOfPages();
    }
} 

*/