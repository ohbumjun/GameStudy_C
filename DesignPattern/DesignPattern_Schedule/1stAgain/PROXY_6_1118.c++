// Structural Proxy
// - Smater Pointer 

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
using namespace std;

// Smart Pointer 의 경우
// 중간에 delete을 안할 수 있도록, 과정을 추가한 것
// 즉, 실제 포인터 처럼 보이지만,
// 사실은 Proxy 형태를 취하고 있는 것이다 
struct BankAccount
{
  virtual ~BankAccount() = default;
  virtual void deposit(int amount) = 0;
  virtual void withdraw(int amount) = 0;
};

struct CurrentAccount : BankAccount // checking
{
  explicit CurrentAccount(const int balance)
    : balance(balance)
  {
  }

  void deposit(int amount) override
  {
    balance += amount;
  }

  void withdraw(int amount) override
  {
    if (amount <= balance) balance -= amount;
  }

  friend ostream& operator<<(ostream& os, const CurrentAccount& obj)
  {
    return os << "balance: " << obj.balance;
  }

private:
  int balance;
};


void smart_pointers()
{
  BankAccount* a = new CurrentAccount(123);
  a->deposit(321);
  delete a;

  // << will not work if you make this a shared_ptr<BankAccount>
  auto b = make_shared<CurrentAccount>(123);

  // CurrentAcount != Bank Account
  // 하지만, 실제 Bank Account 처럼 동작 
  BankAccount* actual = b.get(); // pointer's own operations on a .
  b->deposit(321); // underlying object's operations are on ->
                   // note this expression is identical to what's above
  cout << *b << endl;
  // no delete 도 필요 없다.

  // see shared_ptr in file structure window
}

int main()
{
  communication_proxy();

  getchar();
  return 0;
}
