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

// test�� ���� �츮�� �� �� �ִ� singleton�� ������ �����ϱ�
// �Ʒ��� ���, db �Ϻ��� �����Ͱ�,�츮�� ������ ����
// �´����� ���� ��
// ��, Singleton instance�� �ùٸ���, db ������ ����������
// ���� ��. ��, unit testing�� �ǵ��� ��
// �׷���, singleton Ư����, db�� coupling�� �ʹ� ���ϱ� ������
// ��¿ �� ����, db ��ü�� �� ����ϰ� �ȴ�
// ��, ��ǻ� intergration test�� �ȴٴ� �� 
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
