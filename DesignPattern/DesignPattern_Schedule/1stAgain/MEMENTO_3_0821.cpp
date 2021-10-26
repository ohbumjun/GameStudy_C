// int32_t:  32비트(4바이트) 크기의 부호 있는 정수형 변수 선언
// uint32_t:  32비트(4바이트) 크기의 부호 없는 정수형 변수 선언

#include <iostream>
#include <stdint.h>
#include <vector>

using namespace std;

class BankAccount
{
    int32_t m_balance{0};
    uint32_t m_current{0};
    struct Memento
    {
        int32_t m_balance;
        Memento(int32_t b) : m_balance(b) {}
    };

    vector<shared_ptr<const Memento>> m_changes;

public:
    BankAccount(const int32_t b) : m_balance(b)
    {
        // make_shared<const Memento>(m_balance) ?
        // Memento* res = new Memento(m_balance);
        // std::shared_ptr<Resource> ptr(res) 2개를 합친 것
        // 즉, m_balance를 인자로 넣어줘서 만든 Memento 객체
        // 에 대한 포인터 변수에 대한 shared pointer를 만들어준다

        // 그렇다면, 왜 emplace_back을 사용하는 것일까 ?
        // push_back을 사용해도 되고, emplace_back을 사용해도 된다.
        // 다만, 둘의 차이가 있다
        // 1) push_back같은 경우, 인자로 l-value를 넣어주면 copy
        // r-value를 넣어주면 move,
        // 즉, a. 외부에서 임시객체 및 메모리 할당
        // b. 해당 vector로 copy
        // c. 임시객체 및 메모리 소멸
        // 3가지의 과정이 일어난다는 것이다

        // 반면
        // emplace_back
        // 1) l-value : copy ( 동일 )
        // 2) r-value : vector 내부에서 contruct
        // 즉, 내부적으로 constructor를 호출해서, 해당 객체를
        // 생성하기 위한 인자 및 값만 전달해주면, 알아서
        // 추가해준다는 것( push_back 과같은 과정 x)

        // 여기에 적용하게 되면
        // make_shared<~>는 r-value이다
        // shared_ptr 값을 인자로 전달해준 것이기 때문이다.
        m_changes.emplace_back(make_shared<const Memento>(m_balance));
    }
    const shared_ptr<const Memento> deposit(int32_t amount)
    {
        m_balance += amount;
        m_changes.emplace_back(make_shared<const Memento>(m_balance));
        return m_changes[m_current++];
    }

    void restore(const shared_ptr<const Memento> &m)
    {
        if (m)
        {
            m_balance = m->m_balance;
            m_changes.emplace_back(m);
            m_current = m_changes.size() - 1;
        }
    }

    const shared_ptr<const Memento> undo()
    {
        if (m_current > 0)
        {
            m_balance = m_changes[--m_current]->m_balance;
            return m_changes[m_current];
        }
        cout << "*** Attempt to run off the end!! ***" << endl;
        return {};
    }

    const shared_ptr<const Memento> redo()
    {
        if ((m_current + 1) < m_changes.size())
        {
            m_balance = m_changes[++m_current]->m_balance;
            return m_changes[m_current];
        }
        cout << "*** Attempt to run off the end!! ***" << endl;
        return {};
    }
    friend ostream &operator<<(ostream &os,
                               const BankAccount &ac)
    {
        return os << "balance : " << ac.m_balance;
    }
};

int main()
{
    BankAccount ba{100};
    ba.deposit(50);
    ba.deposit(25);
    cout << ba << "\n"; // 175
    ba.undo();
    cout << "Undo 1: " << ba << "\n";
    ba.undo();
    cout << "Undo 2: " << ba << "\n";
    ba.redo();
    cout << "Redo 2: " << ba << "\n";
    return EXIT_SUCCESS;
}