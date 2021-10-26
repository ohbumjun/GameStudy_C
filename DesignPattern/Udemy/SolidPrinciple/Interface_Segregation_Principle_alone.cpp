#include<iostream>

// not create interfaces that is too large


using namespace std;

struct Document;
struct IMachine
{
	// Machine �� ���õ� ��� ����� ���⿡ �дٰ� ����
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

// �׷��� ����
// ������ printer, ������ scanner ���� �ʿ��� ��쿡��
// ��� �ؾ��ұ� ?
// �̷��� override �ϰ�, Scan ��ɿ��� ����� ������ �Ǵ� ������
// ���� ����, ��� �Ǵ� ��踦
// �䱸���� ���� ���� �ִ�  
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
	// decorator �� �ۼ��ϱ�  
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
