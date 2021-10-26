// 메멘토
// 객체의 상태를 "내부적으로" 저장했다가
// 언제라도 그 상태로 복구가능하게 하는 패턴

// Undo가 가능한 패턴
// Command : 행위의 취소를 통한 Undo
// Memento : 객체의 스냅샷을 보관했다가 Undo

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Machine
{
    int stateA;
    int stateB;
    int temp;
    class Memento
    {
        // RestoreMemento에서
        // Memento class의
        // stateA, stateB에 접근하는 것을 허용하기 위해
        friend class Machine;
        int stateA, stateB;

    public:
        Memento(int a, int b) : stateA(a), stateB(b) {}
    };
    vector<Memento *> back_up;

public:
    int CreateMemento()
    {
        Memento *m = new Memento(stateA, stateB);
        back_up.push_back(m);
        return back_up.size() - 1;
    }
    void SetState(int a, int b)
    {
        stateA = a;
        stateB = b;
    }
    void RestoreMemento(int token)
    {
        Memento *m = back_up[token];
        stateA = m->stateA;
        stateB = m->stateB;
    }
};

int main()
{
    Machine m;
    m.SetState(10, 20);

    // m의 상태를 저장히기 위해서
    // getXXX함수를 만들면, 캡슐화 위배
    //int stateA = m.GetStateA();
    int token = m.CreateMemento();
    m.SetState(30, 40);
    // 이전상태로 복구한다.
    m.RestoreMemento(token);
}