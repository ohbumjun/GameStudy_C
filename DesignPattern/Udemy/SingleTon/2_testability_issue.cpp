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

struct SingletonRecordFinder
{
	int total_population(vector<string> names)
	{
		int result(0);
		for(auto& name : names)
			result += SingletonDatabase::get().get_population(name);
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
TEST(RecordFinderTests, SingletonTotalPopulationTest)
{
	SingletonRecordFinder rf;
	vector<string>names{"Seoul","Mexico city"};
	int tp = rf.total_population(names);
	EXPECT_EQ(175000000+174000000,tp);
}

int main()
{
	testing::InitGoogleTest(&ac,av);
	return RUN_ALL_TESTS();
}
