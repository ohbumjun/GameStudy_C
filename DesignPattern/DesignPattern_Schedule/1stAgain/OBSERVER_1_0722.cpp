//옵저버 패턴

//=> 변동사항을 관련있는 모든 객체에거 전달 할 수 있는 패턴

//문제
//	=> 엑셀의 차트 프로그램을 생각해봅시다.
//	=> 값이 변동되면 차트의 데이터가 실시간으로 업데이트 되야합니다.
//	=> 차트는 여러개일 수도 있습니다.
#include <iostream>
#include <memory.h>
#include <vector>

using namespace std;

#define DATA_LENGTH 5

struct IObserver
{
    virtual void Update(const int *data) = 0;
    virtual ~IObserver() {}
};

class PieGraph : public IObserver
{
public:
    void Update(const int *data)
    {
        cout << "Pie Graph" << endl;
        for (int i = 0; i < DATA_LENGTH; i++)
        {
            cout << data[i] << endl;
        }
    }
};

class BarGraph : public IObserver
{
public:
    void Update(const int *data)
    {
        cout << "Bar Graph" << endl;
        for (int i = 0; i < DATA_LENGTH; i++)
        {
            cout << data[i] << endl;
        }
    }
};

//
struct IObservable
{
    virtual IObserver *Add(IObserver *obs) = 0;
    virtual void Remove(IObserver *obs) = 0;
    virtual void Notify() = 0;
    virtual ~IObservable() {}
};

class Table : public IObservable
{
private:
    int datas[5];
    vector<IObserver *> graphs;

public:
    Table()
    {
        memset(datas, 0, sizeof(int) * DATA_LENGTH);
    }
    virtual IObserver *Add(IObserver *obs) override
    {
        cout << "hello" << endl;
        graphs.emplace_back(obs);
        return obs;
    }
    virtual void Remove(IObserver *obs) override
    {
        int sz = graphs.size();
        std::vector<IObserver *>::iterator iter = graphs.begin();
        for (; iter != graphs.end(); ++iter)
        {
            cout << "obs : " << (*iter) << endl;
            cout << "value : " << (*iter) << endl;
            if (*iter == obs)
            {
                graphs.erase(iter);
                break;
            }
        }
    }
    virtual void Edit()
    {
        int cmd;
        while (true)
        {
            cout << "Index : ";
            cin >> cmd;
            if (cmd < 0 || cmd > 5)
                continue;
            if (cmd == 5)
                break;
            cout << "val : ";
            cin >> datas[cmd];
            Notify();
        }
    }
    virtual void Notify()
    {
        int sz = graphs.size();
        for (int i = 0; i < sz; i++)
            graphs[i]->Update(datas);
    }
};

int main()
{
    Table table;
    IObserver *pieGraph = table.Add(new PieGraph);
    IObserver *barGraph = table.Add(new BarGraph);
    table.Notify();
    table.Remove(pieGraph);
    table.Notify();
    return 0;
}