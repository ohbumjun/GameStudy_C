//템플릿 메소드 패턴이란 ?
//	=> 알고리즘의 기본 골격은 바꾸지 않는다
// ->public으로 정의한다
//	=>변하는 부분은 virtual로 사용하고 자식 클래스에서 정의 할 수 있도록 한다.->private으로 정의한다.
//	=> NVI(Non Virtual Interface)라고도 한다
//	=> 알고리즘의 기본틀은 바뀌지 않고 특정 부분만 바뀌게 되는 효과가 있다(특정부분은 가상함수이므로 자식에서 구현할 수 있지만, 외부에서 호출할 수 없다)

#include <iostream>

using namespace std;

class Shape
{
public:
    int type;
    // 변하지 않은 전체적인 흐름(공통성)에서 변해야 하는 부분(가변성)
    // 을 뽑아 내야 한다.
    // 변해야 하는 것은 가상함수로 뽑아낸다.
protected:
    virtual void DrawImp() { cout << "Shape Draw" << endl; }

public:
    //final    // java에서 이 함수를 자식이 재정의 못하게 할때 사용
    void Draw() // C++11에는 final 이 추가. 함수뒤에 붙인다.
    {
        cout << "Mutex Lock" << endl;
        DrawImp();
        cout << "Mutex Unlock" << endl;
        cout << endl;
    }
    // 자신의 복사본을 만드는 가상함수가 있으면 편리
    virtual Shape *Clone() { return new Shape(*this); }
}

class Rect : public Shape
{
public:
    Rect() { type = 1; }
    virtual void DrawImp() { cout << "Rect Draw" << endl; }
    virtual Shape *Clone() { return new Rect(*this); }
}

class Circle : public Shape
{
    Circle() { type = 2; }
    virtual void DrawImp() { cout << "Circle Draw" << endl; }
    virtual Shape *Clone() { return new Circle(*this); }
}

class Triangle : public Shape
{
public:
    virtual void DrawImp() { cout << "Triangle Draw" << endl; }
    virtual Shape *Clone() { return new Triangle(*this); }
}

int
main()
{
    // 다음 함수에서 문제점은?
    // 메모리 관리가 제대로 안되고 있다

    //대책
    //	 => 스마트 포인터 활용
    //	 => 직접 관리
    Shape *shape = new Rect;
    shape->Draw();

    shape = new Circle;
    shape->Draw();

    shape = new Triangle;
    shape->Draw();
    return 0;
}