#pragma once
//of promise object by return value of the callback function
//of std::async and you can fetch it by its
// associated future object
#include <iostream>
#include <thread>
#include <future>

/*
std::async �� ��

1) std::thread �� ���� ������ �����带 ��������� �Ѵٴ� ��
2) std::move �� ���� ���·� ���ڸ� �Ѱ���� �Ѵٴ� ��

��. 2���� ������ ���� �ִ� �� ����.
*/

using namespace std::chrono;
using namespace std;

class Test
{
public:
    int operator()(int a, int b)
    {
        int res = a + b;
        return res;
    }
};

int addition(int a, int b)
{
    int res = a + b;
    return res;
}


class PackagedTask
{
    // Callback Function
    void ExecuteCallback()
    {
        std::packaged_task<int(int, int)> pk_task(addition);
        std::future<int> futureObj = pk_task.get_future();

        std::thread threadObj(std::move(pk_task), 10, 5);
        threadObj.join();
        int val = futureObj.get();
    }

    // Function Object
    void ExecuteFunctionObject()
    {
        std::packaged_task<int(int, int)> pk_task(std::move(Test()));
        std::future<int> futureObj = pk_task.get_future();
        std::thread threadObj(std::move(pk_task), 10, 5);
        threadObj.join();
        int val = futureObj.get();
        cout << "val is " << val << endl;
    }

    // lambda ����
    void ExecuteLambda()
    {
        std::packaged_task<int(int, int)> pk_task([](int a, int b) {
            return a + b;
        });
        std::future<int> futureObj = pk_task.get_future();

        std::thread threadObj(std::move(pk_task), 10, 5);
        threadObj.join();

        int val = futureObj.get();
        cout << "val using lambda function is " << val << endl;
    }
};

