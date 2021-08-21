// int32_t:  32비트(4바이트) 크기의 부호 있는 정수형 변수 선언
// uint32_t:  32비트(4바이트) 크기의 부호 없는 정수형 변수 선언

#include <iostream>
#include <stdint.h>
#include<vector>

using namespace std;

class BankAccount
{
    int32_t m_balance{ 0 };
    uint32_t m_current{ 0 };
    struct Memento
    {
        int32_t m_balance;
        Memento(int32_t b) : m_balance(b){}
    };

    vector<shared_ptr<const Memento>> m_changes;

public :
    BankAccount(const int32_t b) : m_balance(b)
    {
        // make_shared<const Memento>(m_balance) ?
        // Memento* res = new Memento(m_balance);
        // std::shared_ptr<Resource> ptr(res) 2개를 합친 것
        // 즉, m_balance를 인자로 넣어줘서 만든 Memento 객체
        // 에 대한 포인터 변수에 대한 shared pointer를 만들어준다

        // 그렇다면, 왜 emplace_back을 사용하는 것일까 ?
        // 
        m_changes.emplace_back(make_shared<const Memento>(m_balance));
    }
    const shared_ptr<const Memento> deposit(int32_t amount)
    {

    }
};



int main()
{
    return 0;
}