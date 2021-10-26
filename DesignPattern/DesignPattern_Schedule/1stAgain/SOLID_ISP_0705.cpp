#include <iostream>
/*
>> Not create interfaces which are 
too large and which require the
implementators to maybe
implement too much 

>> SubClass should not be forced to 
depend on methods that does not use 

class Beverage
{
    cost();
    hasMilk(); // ...
}

class Tea : public Beverage
{
    ~ 
    // what if some methods in Beverage
    // is not needed in Tea ? 
    // then we are breaking interface principle
}

*/
using namespace std;

struct Document;

// rather than putting scan,print,fax
// operations in Single Interface,
// you put them into seperate interfaces

// that is it is better to have multiple
// smaller interfaces than a single large one

struct IScanner
{
    virtual void scan(Document &doc){};
};
struct Scanner : IScanner
{
    void scan(Document &doc) override{};
};
struct IPrinter
{
    virtual void print(Document &doc){};
};
struct Printer : IPrinter
{
    void print(Document &doc) override{};
};
struct IFax
{
    virtual void fax(Document &doc){};
};
struct Fax : IFax
{
    void fax(Document &doc) override {}
};
struct IMachine : IScanner, IPrinter, IFax
{
};

struct Machine : IMachine
{
    IPrinter &printer;
    IScanner &scanner;
    IFax &fax;
    Machine(IPrinter &printer, IScanner &scanner)
        : printer(printer), scanner(scanner), fax(fax) {}
    void print(Document &doc) override
    {
        printer.print(doc);
    }
    void scan(Document &doc) override
    {
        scanner.scan(doc);
    }
    void fax(Document &doc) override
    {
        fax.fax(doc);
    }
};