#include<iostream>
#include<memory.h>
#include<vector>

using namespace std;

#define DATA_LENGTH 5

struct IObserver
{
	virtual void Update(const int* data) = 0;
	virtual ~IObserver(){}
};

class PieGraph : public IObserver
{
public :
	void Update(const int* data)
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
	void Update(const int * data)
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
	virtual IObserver* Add(IObserver* obs) = 0;
	virtual void Remove(IObserver* obs) = 0;
	virtual void Notify() = 0;
	virtual ~IObservable(){}
};

class Table : public IObservable
{
private :
	int* datas;
	vector<IObserver*> graphs;
public :
	Table()
	{
		memset(datas, 0, sizeof(int) * DATA_LENGTH);
	}
	virtual IObserver* Add(IObserver* obs) override
	{
		graphs.emplace_back(obs);
	}
	virtual void Remove(IObserver* obs) override
	{
		int sz = graphs.size();
		std::vector<IObserver*>::iterator iter = graphs.begin();
		for (; iter != graphs.end(); ++iter)
		{
			if (*iter == obs) graphs.erase(iter);
		}
	}
	virtual void Edit()
	{
		int cmd;
		while (true)
		{
			cout << "Index : ";
			cin >> cmd;
			if (cmd < 0 || cmd > 5) continue;
			if (cmd == 5) break;
			cout << "val : ";
			cin >> datas[cmd];
			Notify();
		}
	}
	virtual void Notify()
	{
		int sz = graphs.size();
		for (int i = 0; i < sz; i++) graphs[i]->Update(datas);
	}
};


int main()
{
	return 0;
}