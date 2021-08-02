#pragma once

// 클래스를 템플릿 클래스로 만들어주게 되면 .h 에 구현부분까지 모두 함께 만들어주어야 한다.
template <typename T, int Size = 999>
class CTemplate
{
public:
    CTemplate()
    {
        std::cout << typeid(T).name() << std::endl;
        std::cout << "Size : " << Size << std::endl;
    }

    ~CTemplate()
    {
    }

private:
    T       m_T1;
    T       m_T2;

public:
    // 클래스의 멤버함수마다 따로 template 을 지정할수도 있다.
    template <typename T1>
    void TestTemplateFunction()
    {
        std::cout << "Function Template Type : " << typeid(T1).hash_code() << std::endl;
        //std::cout << "Function Template Type : " << typeid(T1).name() << std::endl;
    }
};

