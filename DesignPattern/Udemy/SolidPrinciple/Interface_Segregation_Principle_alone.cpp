#include<iostream>

// not create interfaces that is too large


using namespace std;

struct Document;
struct IMachine
{
	// Machine 과 관련된 모든 기능을 여기에 둔다고 하자
	virtual void print(Document& doc) = 0; 
	virtual void scan(Document& doc)  = 0;
	virtual void fax(Document &doc)   = 0;
}

struct MFP : IMachine
{
	virtual void print(Document& doc) override {}; 
	virtual void scan(Document& doc) override {}
	virtual void fax(Document &doc) override {}	
}

// 그런데 만약
// 오로지 printer, 오로지 scanner 만이 필요한 경우에는
// 어떻게 해야할까 ?
// 이렇게 override 하고, Scan 기능에만 기능을 넣으면 되는 하지만
// 실제 고객은, 모두 되는 기계를
// 요구하지 않을 수도 있다  
struct Scanner : IMachine
{
	virtual void print(Document& doc) = 0; 
	virtual void scan(Document& doc)  = 0;
	virtual void fax(Document &doc)   = 0;	
}

// -------------------------------------------------------------
struct IPrinter
{
	virtual void print(Document &doc) = 0;
};
struct IScanner
{
	virtual void scan(Document &doc) = 0;
}; 
struct Scanner : IScanner {};
struct IFax
{
	virtual void fax(Document &doc) = 0;
};  
struct Printer : IPrinter
{
	void print(Document &doc) override {}
};

struct IMachine : IPrinter, Iscanner {};

struct Machine : Imachine {
	// decorator 로 작성하기  
	IPrinter& printer;
	IScanner& scanner;
	void print(Document &doc) overrride {
		printer.print(doc);
	}
	void scan(Document &doc) override {
		scanner.scan(doc);
	}
};

int main()
{
	return 0;
}
