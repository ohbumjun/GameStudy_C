// int32_t:  32��Ʈ(4����Ʈ) ũ���� ��ȣ �ִ� ������ ���� ����
// uint32_t:  32��Ʈ(4����Ʈ) ũ���� ��ȣ ���� ������ ���� ����

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
        // std::shared_ptr<Resource> ptr(res) 2���� ��ģ ��
        // ��, m_balance�� ���ڷ� �־��༭ ���� Memento ��ü
        // �� ���� ������ ������ ���� shared pointer�� ������ش�

        // �׷��ٸ�, �� emplace_back�� ����ϴ� ���ϱ� ?
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