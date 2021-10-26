#include <iostream>

using namespace std;

class Number;

class Memento
{
public:
    Memento(int val)
    {
        _state = val;
    }

private:
    friend class Number;
    int _state;
};

class Number
{
public:
    Number(int value) : _value(value) {}
    void dubble() { _value *= 2; }
    void half() { _value /= 2; }
    int getValue() { return _value; }
    Memento *createMemento()
    {
        return new Memento(_value);
    }
    void reinstateMemento(Memento *mem)
    {
        _value = mem->_state;
    }

private:
    int _value;
};

class Command
{
public:
    // Action : Number class의 void 리턴형 멤버함수
    // 를 가리키는 함수 포인터
    typedef void (Number::*Action)();
    Command(Number *receiver, Action action)
    {
        _receiver = receiver;
        _action = action;
    }
    virtual void execute()
    {
        _mementoList[_numCommands] = _receiver->createMemento();
        _commandList[_numCommands] = this;
        if (_numCommands > _highWater)
            _highWater = _numCommands;
        +_numCommands++;
        (_receiver->*_action)();
    }
    static void undo()
    {
        if (_numCommands == 0)
        {
            cout << "*** Attempt to run off the end!! ***" << endl;
            return;
        }
        _commandList[_numCommands - 1]->_receiver->reinstateMemento(_mementoList[_numCommands - 1]);
        _numCommands--;
    }
    static void redo()
    {
        // 이렇게 한계선을 정해두는 이유는
        // undo, redo가 무한번 가능하기 때문에
        // 무한번이 아니라, 특정 정도로 제한하여 사용하게 하려고
        if (_numCommands > _highWater)
        {
            cout << "*** Attempt to run off the end!! ***" << endl;
            return;
        }
        (_commandList[_numCommands]->_receiver->*(_commandList[_numCommands]->_action))();
        _numCommands++;
    }

protected:
    Number *_receiver;
    Action _action;
    static Command *_commandList[20];
    static Memento *_mementoList[20];
    static int _numCommands;
    static int _highWater;
};

// 정적 배열 : 외부 초기화 필요
// ex) 배열 초기화 방법
// int arr[] = {1,2,3,4,5}
// 위와 같이, 크기를 명시해주지 않고, 배열의 요소 정의가능
// arr 변수의 크기는, 개발자가 직접 명시하지 않아도
// 자동으로 10으로 지정된다.
Command *Command::_commandList[];
Memento *Command::_mementoList[];
int Command::_numCommands = 0;
int Command::_highWater = 0;

int main()
{
    int i;
    cout << "Integer : ";
    cin >> i;
    Number *object = new Number(i);

    Command *commands[3];
    commands[1] = new Command(object, &Number::dubble);
    commands[2] = new Command(object, &Number::half);

    while (1)
    {
        cout << "Exit[0], Double[1], Half[2], Undo[3], Redo[4]" << endl;
        cin >> i;
        if (i < 0 || i > 4)
        {
            cout << "try again" << endl;
            continue;
        }
        if (i == 3)
            Command::undo();
        else if (i == 4)
            Command::redo();
        else
            commands[i]->execute();
        cout << "value :  " << object->getValue() << endl;
    }
}