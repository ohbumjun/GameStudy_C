// Q. 연산자 오버로딩의 의미

// operator 키워드오, 연산자.를 묶어서 함수의 이름을 정의하면
// 함수의 이름을 이용한 함수의 호출 뿐만 아니라, 연산자를 이용한 함수의 호출도 허용하겠다. 

// Q. 연산자 오버로딩 : "~~" 까지 빼앗을 수는 없다

// 연산자의 순수 기능까지 빼앗을 수는 없다.
int operator + (const int num1, const int num2)
{
    return num * num2;
}

// int 형 데이터의 + 연산은 이미 정해져있다
// 이를 변경하는, 위와 같은 함수의 정의는 허용되지 않는다
// 연산자의 기본 기능을 변경하는 형태의 연산자 오버로딩은 허용되지 않는다.

// Q. 연산자 오버로딩이라는 이름이 붙은 이유
class Point{}; 
int num = 3 + 4;
Point pos = pos1 + pos2;

// 함수가 오버로딩 되면, 전달되는 인자에 따라서 호출되는 함수가 달라진다
// 이와 유사하게, 연산자가 오버로딩 되면, 피연산자듸 종류에 따라서 연산자의 방식이 달라진다.
// 그래서, 연산자 오버로딩이라고 하는 것이다.

// Q. 단한 연산자 오버로딩 2가지 형태  ----
// ++pos
// 1) pos.operator ++(); 멤버함수 형태 오버로딩
// 2) operator ++ (pos); 전역함수 형태 오버로딩 

// Q. 단항 연산자 오버로딩 ----- 예시 코드 만들어보기 (+, -)
class Point
{
private :
    int x, y;
public :
    Point(int px, int py) : x(px), y(py){}
};


// 답변 
class Point
{
private :
    int x, y;
public :
    Point(int px, int py) : x(px), y(py){}
    Point& operator ++()
    {
        x++;
        y++;
        return *this;
    }
    // friend 처리를 해준다.
    friend Point& operator -- (Point& ref);
};

Point& operator -- (Point& ref)
{
    ref.x--;
    ref.y--;
    return ref;
}


// Q. 아래코드가 동작하는 방식 및 과정
int main()
{
    Point(1,2) pos;
    ++(++pos);
    --(--pos);
    return 0;
}

// 답변 -----
int main()
{
    Point(1,2) pos;
    ++(++pos);
    // 1. (++pos) == pos.operator++()
    // 2. *this 반환 + 참조형태 반환 
    // => ++(pos 의 참조값)
    // 3. (pos 참조값).operator++();

    --(--pos);
    // 1. (--pos) --> --(operator--(pos))
    // 2. --(pos 참조값)
    // 3. operator -- (pos 참조값)                                                                                                                                                                                         

    return 0;
}

// Q. 후위 증가 형식 및, 위 Point Class 에 전위 및 후위 증가 적용하기 ---

// ++pos -> pos.operator++() : 전위 증가 
// pos++ -> pos.operator ++(int) : 후위 증가 

class Point
{
    int x,y;
public :
    Point(int px = 0, int py = 0) : x(px), y(py){}

    Point& operator ++ ()
    {
        x++;
        y++;
        return *this;
    }
    // 후위
    const Point operator ++ (int) // 인자에 int를 붙임으로써, 후위증가임을 명시해준다.
    {
        // 후의 증가의 경우, 해당 값은 변하지 않아야 하고
        // 그 다음값부터 바뀌어야 한다
        // 해당 값은 변하지 않게 세팅해주기 위해서 복사본 const를 만들어두고, 이를 return하는 개념을 적용한다.
        const Point obj(x,y);
        x++;
        y++;
        return obj;
    };
    void ShowInfo()
    {
        cout << x << y << endl;
    }
    friend Point& operator --(Point& ref);
    friend const Point operator --(Point& ref, int);
};

Point& operator -- (Point& ref)
{
    --ref.x;
    --ref.y;
    return *this;
};

// 후위 증가
const Point operator -- (Point& ref, int)
{
    const Point obj(ref.x,ref.y);
    --ref.x;
    --ref.y;
    return obj;
}


// Q. 전역 멤버형태의 후의 증가에서 const를 붙여준 이유 + 잘못된 답변 
const Point operator -- (Point& ref, int)
{
    const Point obj(ref.x,ref.y);
    --ref.x;
    --ref.y;
    return obj;
}

// 반환의 대상이 되는 obj 객체가 const로 선언이 되어 있어서 ?
// 아니다. obj 객체가 반환되면, 반환의 과정에서, 해당 obj객체를 복사생성자의 인자로 하여
// 복사 생성되는 임시 객체가 만들어지므로, obj 객체의 const 선언 유무는 중요하지 않다
// operator -- 함수의 반환으로 생성되는 임시객체를 const 객체로 만들어주겠다 ! 라는 의미이다.
// 해당 임시객체의 데이터 변환을 허용하지 않겠다는 의미이다
// 따라서 해당 객체에서는 const 멤버 함수 호출만을 허용한다.

// Q. 아래 코드가 먹히지 않는 이유
int main()
{
    Point pos(1,2);
    (pos++)++;
}

// 왜냐하면, pos++ 을 통해 리턴되는 객체는, const 임시객체이고,
// const 임시 객체에 대해서 변환을 가해주지 못하기 때문이다. 
// 즉, (Point형 const 임시객체)++ 형태를 취해주게 되면 문제가 발생하게 된다는 의미이다.

// Q. 아래에서 Point posAfter 를 const Point posAfter 로 받아주지 않아도 되는 이유 
int main()
{
    Point pos(1,2);
    Point posAfter = pos++;
    posAfter.ShowInfo(); // 1,2
    pos.ShowInfo(); // 2.3

    // pos++ 를 통해 return 되는 임시객체는 const 객체이다.
    // 하지만, 해당 임시객체의 값들을 복사생성자 인자로 하여, posAfter라는 객체를 복사생성하였다
    // 값만 복사해서 들어온 것이므로, posAfter 객체 자체는 const 객체일 필요가 없는 것이다
}


// Q. 곱셈 연산자 오버로딩 ---
// 아래 코드가 되지 않는 이유 + 해결책 
class Point
{
private :
    int x, y;
public :
    Point(int px = 0, int py = 0) : x(px) ,y(py){}
    Point operator * (int time)
    {
        return Point(x * time, y * time); 
    } 
};

int main()
{
    Point pos(1,2);
    Point cpy;
    cpy = 3 * pos;
    // cpy = pos * 3; 이건 된다
}

// >> 답변 
// 불가능한 이유는, 멤버함수의 형태로, 연산자 오버로딩을 정의하면
// 멤버함수가 정의된 클래스의 객체가 오버로딩 된 연산자의 "왼편"에 와야하기 때문이다. 

// >> 해결책 
// 전역함수의 형태로 곱셈 연사자를 오버로딩할 수 밖에 없다. 
Point operator * (int time, Point& ref)
{
    Point pos(ref.x * time , ref.x * time);
    return pos;
}

#include<iostream>

class Point
{
private :
    int x, y;
public :
    Point(int px = 0, int py = 0) : x(px) ,y(py){}
    Point operator * (int time)
    {
        return Point(x * time, y * time); 
    } 
    friend Point operator *(int time, Point& ref);
};

Point operator * (int time, Point& ref)
{
    Point pos(ref.x * time , ref.x * time);
    return pos;
}

int main()
{
    Point pos(1,2);
    Point cpy;
    cpy = 3 * pos;
    // cpy = pos * 3;
}