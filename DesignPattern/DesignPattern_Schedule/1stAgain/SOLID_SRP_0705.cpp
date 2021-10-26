// Single Responsibility Principle
// �ϳ��� class�� �ϳ��� �ֵ� responsibility only ! 

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<boost/lexical_cast.hpp>

using namespace std;

struct Journal
{
	string title;
	vector<string> entries;
	Journal(const string& title) : title(title){}
	void add(const string& title);
};

void Journal::add(const string& title)
{
	static int count = 1;
	entries.push_back(boost::lexical_cast<string>(count++)
		+ " : " + title);
};

struct PersistanceManager
{
	static void save(const Journal& j, const string& filename)
	{
		ofstream ofs(filename);
		for (auto& e : j.entries)
			ofs << e << endl;
	}
};

/*

int main()
{
	Journal journal("Dear Diary");
	Journal add("I ate a bug");
	Journal add("I cried today");

	PersistanceManager pm;
	pm.save(journal, "diary.txt");
}

*/