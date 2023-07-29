//To make thread object as member variable of class
#include <iostream>
#include <thread>
#include <vector>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <future>
#include <algorithm>

using namespace std;

class ThreadTest
{
    std::thread m_threadObj;
public:
    //deleting copy constructor
    ThreadTest(const ThreadTest&) = delete;

    //deleting assignment operator
    ThreadTest& operator=(const ThreadTest&) = delete;

    //parameterized constructor (�����尡 ������ �Լ� �Ѱ��ֱ�)
    ThreadTest(std::function<void()> func);

    //move constructor
    ThreadTest(ThreadTest&& obj);

    //move assignment operator
    ThreadTest& operator=(ThreadTest&& obj);

    //Destructor
    ~ThreadTest();
};

ThreadTest::ThreadTest(std::function<void()> func) : m_threadObj(func)
{
}

//implementing move constructor
ThreadTest::ThreadTest(ThreadTest&& obj)
{
    cout << " move constructor " << endl;
    m_threadObj = std::move(obj.m_threadObj);
}

//implementing move assignment operator
ThreadTest& ThreadTest::operator=(ThreadTest&& obj)
{
    cout << " move assignment operator " << endl;

    if (m_threadObj.joinable())
    {
        m_threadObj.join();
    }

    m_threadObj = std::move(obj.m_threadObj);
    return *this;
}

// ���ܰ� ���� ���� ����Ͽ�, join �Լ��� ������ ȣ���Ͽ�, �ش� thread ������ ���������� ����� �� �ֵ��� �ؾ� �Ѵ�.
ThreadTest::~ThreadTest()
{
    if (m_threadObj.joinable())
    {
        m_threadObj.join();
    }
}

int main()
{
    std::vector<ThreadTest> threadVec;
    std::function<void()> func = []() {
        cout << " lambda func and " << endl;
        cout << " Thread id is " << std::this_thread::get_id() << endl;
    };


    ThreadTest threadObj1(func);//will call callback function i.e.
    //lambda function
    //as soon as thread object is created
    ThreadTest threadObj2(func);
    ThreadTest threadObj3(func);
    ThreadTest threadObj4(func);

    threadVec.push_back(std::move(threadObj1));//will call move constructor
    threadVec.push_back(std::move(threadObj2));
    threadVec.push_back(std::move(threadObj3));

    threadVec[2] = std::move(threadObj4);//here
    //move assignment operator
    //is called

//threadVec.push_back(std::move(ThreadTest threadObj3(func)));
    return 0;
}
