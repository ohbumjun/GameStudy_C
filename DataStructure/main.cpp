// Composite PAttern
// single object ~ multiple of single object로 구성된 object
// 같은 interface 을 따르게 해서
// treat them uniformly 하고 싶은 것이다

#include<iostream>
#include<vector>
#include<string>

struct Item
{
	std::string m_Name;
	Item(std::string Name) :m_Name(Name) {};
	virtual int getSize() = 0;
};

struct File : public Item
{
	int m_Size;
	File(std::string Name, int Size) : Item(Name),m_Size(Size){}
	virtual int getSize() override { return m_Size; }
};

struct Folder : public Item
{
	std::vector<Item*> m_Vector;
	Folder(std::string Name) : Item(Name)
	{
	}
	~Folder()
	{
		for (size_t i = 0; i < m_Vector.size(); i++)
			delete m_Vector[i];
	}
	void addItem(Item* p) { m_Vector.push_back(p); }
	virtual int getSize() override
	{
		int sz = 0;
		for (size_t i = 0; i < m_Vector.size(); i++)
		{
			sz += m_Vector[i]->getSize();
		}
		return sz;
	}
};

int main()
{
	Folder* root = new Folder("Root");
	Folder* A = new Folder("A");
	File* f1 = new File("a.txt", 10);
	File* f2 = new File("b.txt", 20);

	root->addItem(A);
	root->addItem(f1);
	A->addItem(f2);

	std::cout << f1->getSize() << std::endl;
	std::cout << A->getSize() << std::endl;
	std::cout << root->getSize() << std::endl;

	return 0;
}