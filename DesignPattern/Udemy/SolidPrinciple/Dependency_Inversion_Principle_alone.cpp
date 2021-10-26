#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<fstream>
#include<tuple>

/*
>> Best way to form dependencies between 
different objects

ex) 
Store ~~ Payment Processor ~~ Stripe API

Payment Processor : add piece in the middle 
= an "interface" which includes all of the behavior
that we want Stripe API to do for us  

Stripe API update �� �ϴ� ��� ����
Payment Processor �� �ν��� �� �ִ� ���¸� �������شٸ�
Stripe API�� ���ϴ���, Store�� ������ �ʰ�
���� ����� ������ �� ������ �� �ְ� �� ���̴�  

--------------------------------------------------------------
1) High Level modules, should not depend on low-level modules.
	Both should depend on abstractions
2) Abstractions should not depend on details
	Details should depend on abstractions 
--------------------------------------------------------------

*/

using namespace std;

enum class Relationship
{
	parent,
	child,
	sibling
}

struct Person
{
	string name;
}

// interface 
struct RelationshipBrowser
{
	// pure virtual function
	virtual vector<Person> find_all_children_of(const string& name) = 0;	
};


struct Relationships : RelationshipBrowser // low-level module
{
	vector<tuple<Person, Relationship, Person>> relations;
	void add_parent_and_child(const Person& parent, const Person& child)
	{
		relations.push_back({parent, Relationship::parent, child});
		relations.push_back({child, Relationship::child, parent});
	}
	virtual vector<Person> find_all_children_of(const string& name) override
	{
		vector<Person> result;
		for(auto && [first,rel,second] : relations )
		{
			if(first.name == name && rel == Relationship::parent)
			{
				result.push_back(second);
			}
		}
		return result;
	}
};

struct Research // high-level 
{
	// Research --> depend on --> RelationshipBrowser ( low-level-module )
	Research(RelationshipBrowser& browser)
	{
		for(auto& child : browser.find_all_children_of("John"))
		{
			cout << "John has a child called " << child.name << endl;
		}
	}
}

/*
struct Research // high-level 
{
	// Research --> depend on --> Relationships ( low-level-module )
	// direct dependency !!
	// how solve ? put actual code or works to new low-level module
	// which is intensifying abstraction 
	Research(Relationships& relationships)
	{
		auto& relations = relationships.relations;
		for(auto&& [first,rel,second] : relations)
		{
			if(first.name == "John" && rel == Relationships::parent )
			{
				// second�� John�� child ���� ���� �� �� �ִ� .
				cout << "John has a child called " << second.name << endl;
			}
		}
	}
}

*/

int main()
{
	Person parent{"John"};
	Person child1{"Chris"}, child2{"Matt"};
	
	Relationships relationships;
	relationships.add_parent_and_child(parent,child1);
	relationships.add_parent_and_child(parent,child2);
	
	Research _(relationships);
	
	return 0;
}
