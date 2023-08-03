#pragma once

//We need to stop thread, so signaling is required between
//threads by sending future object to new thread and maiking
//future object to wait in loop
//and in main thread when promise object is sending signal to stop thread
// i.e simply set the value value then future object
//will resume and thread will do pending task
//before exiting

#include <iostream>
#include <future>
#include <thread>

using namespace std;

void func(std::future<void> futureObj)
{
    cout << " In callback func " << endl;
    // while (futureObj.wait_for(std::chrono::milliseconds(5)) == std::future_status::timeout)
    while (futureObj.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
    {
        cout << " some task to do in new thread " << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "i have been asked to be killed" << endl;
    cout << "i should finish pending task of deallocation of resources if allocated" << endl;
    cout << "closing file if i have opened, unlocking lock if i acquired lock " << endl;
    cout << "**before new thread exit**" << endl;
}

class StopThread
{
public :
    StopThread() = default;

    void Execute()
    {
        cout << "main thread " << endl;
        std::promise<void> promiseObj;
        std::future<void> futureObj = promiseObj.get_future();
        std::thread threadObj(func, std::move(futureObj));
        std::this_thread::sleep_for(std::chrono::seconds(10));

        cout << " asking thread to exit " << endl;
        promiseObj.set_value();

        // thread 가 자기 일을 할 때까지 기다리게 한다.
        threadObj.join();
        cout << "in main thread exit " << endl;
    }
};

