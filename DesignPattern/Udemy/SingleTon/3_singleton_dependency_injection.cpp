#define _USE_MATH_DEFINES
#include<map>
#include<boost/lexical_cast.hpp>

using namespace boost;

// singleton instance�� ���� db�� 
// �ʹ� ���� coupling�� ������ ���� ������  
// dummy db�� �����
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
		// �� �κ��� �ٷ� Singleton�� �ش��Ѵ�  
			result += SingletonDatabase::get().get_population(name);
		return result;
	}
}

// ��,. ���� dependency injection�� ������ ���̴�
// we keep the database we're working with as dependency
struct ConfigurableRecordFinder
{
	Database &db;
	// �ʱ�ȭ
	ConfigurableRecordFinder(Database &db) : db(db) {} 
	int total_population(vector<string> names)
	{
		int result(0);
		for(auto& name : names)
		// SingletonDatabase::get()
		// �� �κ��� �ٷ� Singleton�� �ش��Ѵ�  
			result += db.get_population(name);
		return result;
	}
}

// test�� ���� �츮�� �� �� �ִ� singleton�� ������ �����ϱ�
// �Ʒ��� ���, db �Ϻ��� �����Ͱ�,�츮�� ������ ����
// �´����� ���� ��
// ��, Singleton instance�� �ùٸ���, db ������ ����������
// ���� ��. ��, unit testing�� �ǵ��� ��
// �׷���, singleton Ư����, db�� coupling�� �ʹ� ���ϱ� ������
// ��¿ �� ����, db ��ü�� �� ����ϰ� �ȴ�
// ��, ��ǻ� intergration test�� �ȴٴ� �� 

// �ذ�å���� �õ��� ���� �� : dependency injection
// you have a point which you can insert the dependency 
// instead of actual object 
TEST(RecordFinderTests, SingletonTotalPopulationTest)
{
	SingletonRecordFinder rf;
	vector<string>names{"Seoul","Mexico city"};
	int tp = rf.total_population(names);
	EXPECT_EQ(175000000+174000000,tp);
}

// ������ unit testing ���� �۵��� ���̴�
// singleton�� ������ �����̞d����
// singleton is bad idea if !
// you have hard dependency on it.
// if you just use its accessor, 
// if you use the getter 

// �ݸ�,
// if you have your test in terms of dependency
// that you can substitute for making something 
// that is more testable, then everthing is fine
 
 // -----------------------------------
// �츮�� �� ���� �����̳�
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
