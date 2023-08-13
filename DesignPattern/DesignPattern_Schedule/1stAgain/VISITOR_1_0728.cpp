#include <iostream>

using namespace std;

//컨테이너 종류와 무관하게 특정 작업을 행하게 하고 싶다.
//	=> 컨테이너는 특정 인터페이스를 제공해야 한다.( Accept )
//	=> 컨테이너를 방문하면서
//  =>특정 작업을 수행하는 객체를 제작해야한다( Visitor )

// 모든 방문자 객체의 인터페이스
template <typename T>
struct IVisitor
{
    virtual void visit(T &a) = 0;
    virtual ~IVisitor() {}
};

// 모든 방문의 대상(컨테이너, 복합객체)의 인터페이스
template <typename T>
struct IAcceptor
{
    virtual void accept(IVisitor<T> *visitor) = 0;
    virtual ~IAcceptor() {}
};

//************************************************************************************************

//사용자는 컨테이너와 무관하게 다음 코드만 작성하면 된다.

// 요소를 2배로 하는 방문자
template <typename T>
class TwiceVisitor : public IVisitor<T>
{
public:
    virtual void visit(T &a) { a *= 2; }
};

// 요소를 출력하는 방문자
template <typename T>
class ShowVisitor : public IVisitor<T>
{
public:
    virtual void visit(T &a) { cout << a << endl; }
};

//요소를 전부 0으로 만드는 방문자
template <typename T>
class ZeroVisitor : public IVisitor<T>
{
public:
    virtual void visit(T &a) { a = 0; }
};

//************************************************************************************************

template <typename T>
struct Node
{
    T data;
    Node *next;
    Node(const T &a, Node *n) : data(a), next(n) {}
};

template <typename T>
class slist : public IAcceptor<T>
{
    Node<T> *head;

public:
    // 아래 함수만 정확히 이해하면 됩니다.
    virtual void accept(IVisitor<T> *visitor)
    {
        Node<T> *current = head;

        while (current)
        {
            visitor->visit(current->data); // 방문자에게 모든요소를전달
            current = current->next;
        }
    }
    slist() : head(0) {}

    void push_front(const T &a) { head = new Node<T>(a, head); }
    T &front() { return head->data; }
};

//************************************************************************************************

int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);

    // s안에 있는 모든 요소를 2배로 하고 싶다.
    // 1. 루프를 수행하면서 모든 요소를 2배로한후 다시 넣으면된다.
    // 2. 특정 작업을 자주 하게 된다면 멤버 함수를 추가하는 것이 좋다.
    //		 s.twiceAll();
    // 3. list뿐 아니라 모든 컨테이너에서 해당 작업을 자주 하게 된다.
    //	  모든 컨테이너의 멤버 함수가 추가되어야 한다.
    //    => 방문자 패턴을 사용하자.

    TwiceVisitor<int> tv; // 컨테이너의 모든요소를 2배로 하는 Visitor
    s.accept(&tv);

    ZeroVisitor<int> zv; // 컨테이너의 모든요소를 0으로 하는 방문자
    s.accept(&zv);

    ShowVisitor<int> sv; // 모든 요소를 출력하는 방문자
    s.accept(&sv);
}