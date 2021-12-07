// Q. "객체 배열" 할당 2가지 형태
// Simple arr[10];
// Simple * ptr = new Simple[10];

// Q. 객체 배열 특성 2가지

// 1) 객체가 모여서 배열을 구성, 배열을 선언하는 경우에도 생성자는 호출이 되지만 
// 배열의 선언과정에서는 호출할 생성자를 별도로 명시하지 못한다. (생성자에 인자를 전달하지 못한다.)
// 따라서 위의 형태로 배열이 생성되려면, 아래 형태의 생성자가 정의되어 있어야 한다
// ex) Simple() {...}

// 2) 별도의 초기화 과정을 거쳐야 한다.
int main()
{
    Simple arr[2];

    for (int i = 0; i < 2; i++)
    {
        int cost;
        cin >> cost;
        arr[i].cst = cost;
    }
}

// Q. "객체 포인터 배열" ?

// 객체의 주소값 저장이 가능한 포인터 변수로 이루어진 배열 
Simple * arr[3];


// Q. 아래에서 delete 이 아니라 [] 을 호출해주는 이유 
class Test
{
private :
    int m_Data;
};

int main()
{
    Test* test = new Test[5];

    delete[] test;
}

// 5개의 Test 객체 모두에 대해서 소멸자를 호출해주기 위함이다
// [] 을 붙이지 않는다면, 첫번째 원소에 해당하는 객체에 대한 소멸자만을 호출해주게 된다. 
 