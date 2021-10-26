#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Person;
struct ChatRoom
{
    vector<Person *> people;

    void broadcast(const string &origin,
                   const string &message);

    void join(Person *p);

    void message(const string &origin,
                 const string &who,
                 const string &message);
};

// Person
struct Person
{
    string name;
    ChatRoom *room{nullptr};
    vector<string> chat_log;

    Person(const string &name);

    void say(const string &message) const;
    void pm(const string &who,
            const string &message) const;

    void receive(const string &origin,
                 const string &message);

    bool operator==(const Person &rhs) const;

    bool operator!=(const Person &rhs) const;
};

// ChatRoom
void ChatRoom::broadcast(const string &origin, const string &message)
{
    for (auto p : people)
        if (p->name != origin)
            p->receive(origin, message);
}

void ChatRoom::join(Person *p)
{
    string join_msg = p->name + " joins the chat";
    broadcast("room", join_msg);
    p->room = this;
    people.push_back(p);
}

void ChatRoom::message(const string &origin,
                       const string &who,
                       const string &message)
{
    auto target = std::find_if(
        begin(people),
        end(people),
        [&](const Person *p)
        {
            return p->name == who;
        });
    if (target != end(people))
    {
        (*target)->receive(origin, message);
    }
}

Person::Person(const string &name) : name(name) {}

void Person::say(const string &message) const
{
    room->broadcast(name, message);
}

void Person::pm(const string &who, const string &message) const
{
    room->message(name, who, message);
}

void Person::receive(const string &origin, const string &message)
{
    string s{origin + ": \"" + message + "\""};
    std::cout << "[" << name << "'s chat session]" << s << "\n";
    chat_log.emplace_back(s);
}

bool Person::operator==(const Person &rhs) const
{
    return name == rhs.name;
}

bool Person::operator!=(const Person &rhs) const
{
    return !(rhs == *this);
}

int main()
{
    ChatRoom room;

    Person john{"John"};
    Person jane{"Jane"};
    room.join(&john);
    room.join(&jane);
    john.say("hi room");
    jane.say("oh, hey john");

    Person simon{"Simon"};
    room.join(&simon);
    simon.say("hi everyone!");

    jane.pm("Simon", "glad you found us, simon!");

    return 0;
}