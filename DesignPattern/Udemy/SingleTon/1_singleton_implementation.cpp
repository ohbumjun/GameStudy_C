#define _USE_MATH_DEFINES
#include<map>
#include<boost/lexical_cast.hpp>

using namespace boost;

class SingletonDatabase
{
	SingletonDatabase()
	{
		// get file, read data
		cout << "Initializing database\n" << endl;
		ifstream ifs("capitals.txt");
		string s, s2;
		
		while(getline(ifs,s))
		{
			getline(ifs,s2);
			int pop = lexical_cast<int>(s2);
			capitals[s] = pop;
		}
	}
	map<string,int> capitals;
	public :
		// below 2 codes : prevent replicates
		// prevent copy
		SingletonDatabase(SingletonDatabase const&) = delete;
		// = operator 
		void operator=(SingletonDatabase const&) = delete;
		
		static SingletonDatabase& get()
		{
			static SingletonDatabase db;
			return db;
		}
		
		int get_population(const strin& name)
		{
			// return actual population
			return capitals[name];
		}
}

int main()
{
	// 아래는 불가하다,위에서 = 을 막았기 때문이다 ?
	// auto db = SingletonDatabase::get();
	string city = "Tokyo";
	
	cout << city << " has population " <<
 	SingletonDatabase::get().get_population(city);
	return 0;
}
