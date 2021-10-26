// Single Responsibility Principle
// 하나의 class는 하나의 주된 responsibility only ! 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

struct Journal
{
	string title;
	vector<string> entries;
	
	Journal(const strig &title) : title(title){}
	void add_entry(const string & entry)
	{
		static int count = 1;
		// lexical_cast<type>(value)
		// string, int간의 변환 가능
		//  
		entries.push_back(lexical_cast<string>(count++)+" : "+entry);
	}
	void save(const string& filename)
	{
		ofstream ofs(filename);
		for(auto& e : entries)
			ofs << e << endl;
	}
}

// 따로 저장을 위한 부분을 정리한다
// Journal s struct에 두는 것이 아니라  
struct PersistenceManager
{
	static void save(const Journal &j, const string& filename)
	{
		ofstream ofs(filename);
		for(auto& e : entries)
			ofs << e << endl;
	}
}

int main()
{
	Journal journal("Dear Diary");
	journal.add_entry("I ate a bug");
	journal.add_entry("I cried today");
	return 0;
}
