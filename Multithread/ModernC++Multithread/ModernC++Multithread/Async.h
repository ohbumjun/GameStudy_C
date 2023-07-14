//Here there are 3 approaches shown
//1st by same thread doing all operations, time taken is double
//2nd appraoch by using std::promise and std::future, time taken is less than half
//3rd approach by using async, its easy mechanism, time taken is less i.e. half

/*#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std;

int subtraction(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int res = a-b;
    return res;
}

int addition(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int res = a+b;
    return res;
}

int main()
{
    cout << "main thread running " << endl;
    system_clock::time_point start = system_clock::now();
    int res1 = addition(10,5);
    int res2 = subtraction(10,5);
    system_clock::time_point endtime = system_clock::now();
    auto duration = duration_cast < std::chrono::seconds > (endtime - start).count();
    std::cout << "Total Time Taken = " << duration << " Seconds" << std::endl;
    std::cout<<"res1 is "<<res1<<" and res2 is "<<res2<<std::endl;

    return 0;
}
*/

//If we use std::promise and std::future, then as below

/*#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>

using namespace std::chrono;
using namespace std;

int subtraction(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int res = a - b;
    return res;
}

void addition(std::promise<int> *promiseObj,int a, int b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int res = a + b;
    promiseObj->set_value(res);
}

int main()
{
    cout << "test " << endl;
    system_clock::time_point start = system_clock::now();
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();
    std::thread threadObj(addition, &promiseObj, 10, 5);

    int res = subtraction(10,5);
    int res2 = futureObj.get();
    system_clock::time_point endtime = system_clock::now();
    auto duration = duration_cast < std::chrono::seconds > (endtime - start).count();
    std::cout << "Total Time Taken = " << duration << " Seconds" << std::endl;

    cout<<"addition i.e. res2 is "<<res2<<" and subtraction res is "<<res<<endl;
    threadObj.join();
    return 0;
}
*/



//by using std::async time taken same as time
//taken using std::future and std::promise
//but using std::async is easy as it internally creates
//separate thread and also create internally
//promise object and will set the value
//of promise object by return value of the callback function
//of std::async and you can fetch it by its
// associated future object
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>

using namespace std::chrono;
using namespace std;

class Async
{
    int subtraction(int a, int b)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        int res = a - b;
        return res;
    }

    int addition(int a, int b)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        int res = a + b;
        return res;
    }

    void Execute()
    {
        cout << "multithreading std::async " << endl;
        system_clock::time_point start = system_clock::now();

        std::future<int> futureObj = std::async(std::launch::async, addition, 10, 5);
        int res2 = subtraction(10, 5);

        //Note get function as shown
        //below should be called at last
        // after performing all other operations of program
        //because it blocks till value is not set
        //and other instruction will not execute
        int res = futureObj.get();

        system_clock::time_point endtime = system_clock::now();
        auto duration = duration_cast <std::chrono::seconds> (endtime - start).count();
        std::cout << "Total Time Taken = " << duration << " Seconds" << std::endl;

        cout << "addition i.e. res is " << res << "and subtraction res2 is " << res2 << endl;
    }
};

