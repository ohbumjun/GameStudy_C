// 복합객체와 개별객체를 동일시 해서 사용하는 패턴
// 즉, 아래의 경우 ,폴더 안에 파일이 들어갈 수도,
// 폴더 안에 또 다른 폴더가 들어갈 수도
// 즉, 폴더는 복합객체이고, 파일은 개별객체인데
// 폴더도, 파일과 마찬가지로, 또 다른 폴더라는
// 복합객체 안에 들어가는 객체로 고려하여 넣는다는 것이다

#if 0
int main()
{
	Folder* root = new Folder("ROOT");
	Folder* A = new Folder("A");
	File* f1 = new File("a.txt", 10);
	File* f2 = new File("b.txt", 20);
	root->addItem(A);
	root->addItem(f1);
	A->addItem(f2);

	cout << f1->getSize() << endl;  // 10, 파일은 자신의 크기가 있습니다.
	cout << A->getSize() << endl;   // 20, 폴더는 자신의 크기는없지만
	cout << root->getSize() << endl;// 30  크기를 구할수는 있습니다.
}
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Item
{
    string name;

public:
    Item(string s) : name(s) {}
    virtual int getSize() = 0;
}

class File : public Item
{
    int size;
    File(string s, int n) : Item(s), size(n) {}
    int getSize() { return size; }
}

class Folder : public Item
{
    vector<Item *> v; // 핵심
public:
    Folder(string n) : Item(n) {}
    virtual ~Folder()
    {
        for (int i = 0; i < v.size(); i++)
            delete v[i];
    }
    void addItem(Item *p) { v.push_back(p); }
    int getSize()
    {
        int sz = 0;
        for (int i = 0; i < v.size(); i++)
            sz += v[i]->getSize();
        return sz;
    }
}

int
main()
{
    Folder *root = new Folder("ROOT");
    Folder *A = new Folder("A");
    File *f1 = new File("a.txt", 10);
    File *f2 = new File("b.txt", 20);
    root->addItem(A);
    root->addItem(f1);
    A->addItem(f2);

    cout << f1->getSize() << endl;   // 10, 파일은 자신의 크기가 있습니다.
    cout << A->getSize() << endl;    // 20, 폴더는 자신의 크기는없지만
    cout << root->getSize() << endl; // 30  크기를 구할수는 있습니다.
}