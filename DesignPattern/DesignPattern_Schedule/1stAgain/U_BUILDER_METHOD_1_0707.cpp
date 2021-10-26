/*
This Time, 
we are going to use 2 builders
*/
#include <string>

using namespace std;

class PersonBuilder;

class Person
{
    // address
    std::string street_address, post_code, city;
    // employment
    string company_name, position;

    int annual_income{0};

public:
    static PersonBuilder create();

    friend class PersonBuilder;
    friend class PersonJobBuilder;
    friend class PersonAddressBuilder;
};

class PersonJobBuilder : public PersonBuilderBase
{
    typedef PersonJobBuilder Self;

public:
    PersonJobBuilder(Person &person) : PersonJobBase(person) {}
    Self &at(std::string company_name)
    {
        person.company_name = company_name;
        return *this;
    }
    Self &as_a(std::string position)
    {
        person.position = position;
        return *this;
    }
    Self &earning(int annual_income)
    {
        person.annual_income = annual_income;
        return *this;
    }
}

class PersonAddressBuilder : public PersonBuilderBase
{
    typedef PersonAddressBuilder Self;

public:
    PersonAddressBuilder(Person &person) : PersonBuilderBase(person) {}
    Self &at(std::string street_address)
    {
        person.street_address = street_address;
        return *this;
    }
    Self &with_postcode(std::string post_code)
    {
        person.post_code = post_code;
        return *this;
    }
    Self &in(std::string city)
    {
        person.city = city;
        return *this;
    }

}

// use
class PersonBuilderBase
{
protected:
    Person &person;

public:
    PersonBuilderBase(Person &person) : person(person) {}

    operator Person() const
    {
        return std::move(person);
    }

    // PersonAddressBuilder, PersonJobBuilder
    // they inherit from person builder based
    // not from person builder
    // the only reason why we have person builder is so that
    // we construct the object that's being created ex) in main()
    // but we only construct it once
    // we don't do it for every inherited because that would
    // result in ludicrous amount of duplication
    PersonAddressBuilder lives() const { return PersonAddressBuilder{person}; }
    PersonJobBuilder works() const { return PersonJobBuilder{person}; }
}

class PersonBuilder : PersonBuilderBase
{
    // Person Builder does not actually take any argument
    // just provides reference
public:
    PersonBuilder() : PersonBuilderBase(p) {}

private:
    Person p;
}

int
main()
{
    // You work with one builder ( PersonbaseBuilder ) basically
    // but because, both builders share the common interface
    // can jump from one to another with ease
    Person p = Person::create()
                   .lives()
                   .at("123 London Road")
                   .with_postcode("SW1 1GB")
                   .in("London")
                   .works()
                   .at("PragmaSoft")
                   .as_a("Consultant")
                   .earning(10e6);
    return 0;
}
