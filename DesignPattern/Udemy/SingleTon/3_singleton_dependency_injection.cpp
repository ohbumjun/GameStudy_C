#define _USE_MATH_DEFINES
#include<map>
#include<boost/lexical_cast.hpp>

using namespace boost;

// singleton instance가 실제 db와 
// 너무 강한 coupling을 가지는 것의 문제점  
// dummy db를 만든다
class Database
{
	public :
		virtual int get_population(const string& name) = 0;
} 

class DummyDatabase : public Database
{
	map<string,int> capitals;
	public :
		DummyDatabase()
		{
			capitals["alpha"] = 1;
			capitals["beta"]  = 2;
			capitals["gamma"] = 3;
		}
	public : 
	int get_population(const string &name) override
	{
		return 0;
	}
 } 

class SingletonDatabase : public Database
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

struct SingletonRecordFinder
{
	int total_population(vector<string> names)
	{
		int result(0);
		for(auto& name : names)
		// SingletonDatabase::get()
		// 위 부분이 바로 Singleton에 해당한다  
			result += SingletonDatabase::get().get_population(name);
		return result;
	}
}

// 자,. 이제 dependency injection을 수행할 것이다
// we keep the database we're working with as dependency
struct ConfigurableRecordFinder
{
	Database &db;
	// 초기화
	ConfigurableRecordFinder(Database &db) : db(db) {} 
	int total_population(vector<string> names)
	{
		int result(0);
		for(auto& name : names)
		// SingletonDatabase::get()
		// 위 부분이 바로 Singleton에 해당한다  
			result += db.get_population(name);
		return result;
	}
}

// test를 통해 우리가 알 수 있는 singleton의 단점은 무엇일까
// 아래의 경우, db 일부의 데이터가,우리가 예상한 값과
// 맞는지를 보는 것
// 즉, Singleton instance가 올바르게, db 정보를 가져오는지
// 보는 것. 즉, unit testing을 의도한 것
// 그런데, singleton 특성상, db와 coupling이 너무 강하기 때문에
// 어쩔 수 없이, db 전체를 다 사용하게 된다
// 즉, 사실상 intergration test가 된다는 것 

// 해결책으로 시도할 만한 것 : dependency injection
// you have a point which you can insert the dependency 
// instead of actual object 
TEST(RecordFinderTests, SingletonTotalPopulationTest)
{
	SingletonRecordFinder rf;
	vector<string>names{"Seoul","Mexico city"};
	int tp = rf.total_population(names);
	EXPECT_EQ(175000000+174000000,tp);
}

// 이제는 unit testing 같이 작동할 것이다
// singleton의 단점이 무엇이엀을까
// singleton is bad idea if !
// you have hard dependency on it.
// if you just use its accessor, 
// if you use the getter 

// 반면,
// if you have your test in terms of dependency
// that you can substitute for making something 
// that is more testable, then everthing is fine
 
 // -----------------------------------
// 우리가 한 것은 무엇이냐
// - extracted interface called database
// - and we implemented this interface in dummy db
// which has predictable data
// - by this, we ended up with unit test 
 
TEST(RecordFinderTests, DependentTotalPopulationTest)
{
	DummyDatabase db;
	ConfigurableRecordFinder rf{db};
	EXPECT_EQ(4,rf.total_population(vector<string>{"alpha","gamma"}));
}

int main()
{
	testing::InitGoogleTest(&ac,av);
	return RUN_ALL_TESTS();
}
