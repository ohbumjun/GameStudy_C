#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    string name;
};

// middle-level
struct RelationshipBrowser
{
    virtual vector<Person *> find_all_children_of(const string &name) = 0;
};

// low-level
struct Relationships : RelationshipBrowser
{
    vector<tuple<Person, Relationship, Person>> relations;
    void add_parent_and_child(Person &parent, Person &child)
    {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }
    vector<Person *> find_all_children_of(const string &name) override
    {
        vector<Person *> result;
        for (auto &&[first, rel, second] : relations)
            if (first.name == name && rel == Relationship::parent)
            {
                result.push_back(second);
            }
    };
    return result;
};

struct Research
{
    Research(RelationshipBrowser &browser)
    {
        for (auto &child : browser.find_all_children_of("John"))
        {
            cout << "John has a child called" << child.name << endl;
        }
    }
};

int main()
{
    return 0;
}