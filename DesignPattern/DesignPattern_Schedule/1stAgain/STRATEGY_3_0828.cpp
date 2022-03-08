#include <cassert>
#include <iostream>
#include <string>

using namespace std;

// strategy interface
class Record
{
public:
    virtual void start_record() = 0;
    virtual void store_field(
        const string &name,
        const string &value) = 0;
    virtual void finish_record() = 0;
    virtual ~Record() {}
};

struct ContactData
{
    string first_name, last_name, phone, email; //
};

// context -> has strategy
class ContactRecorder
{
private:
    Record *m_Record;

public:
    ContactRecorder(Record *a) : m_Record(a)
    {
        // a == 0 이면
        // false를 return 하며
        // 프로그램 종료
        assert(a != 0);
    }
    void store(
        const ContactData &data)
    {
        assert(m_Record != 0);
        m_Record->start_record();
        m_Record->store_field(
            "first name",
            data.first_name);
        m_Record->store_field(
            "last name",
            data.last_name);
        m_Record->store_field(
            "phone",
            data.phone);
        m_Record->store_field(
            "email",
            data.email);
        m_Record->finish_record();
    }
};

class StreamRecord : public Record
{
private:
    ostream &m_ostream;
    string m_record_name;

public:
    StreamRecord(
        ostream &s,
        const string &record_name = "") : m_ostream(s),
                                          m_record_name(record_name) {}
    void start_record()
    {
        m_ostream << m_record_name << "(";
    }
    void store_field(
        const string &name,
        const string &value)
    {
        m_ostream << name << ": " << value << "; ";
    }
    void finish_record()
    {
        m_ostream << ")" << endl;
    }
    void set_record_name(
        const string &name)
    {
        m_record_name = name;
    }
};

class MySql
{
};
class DatabaseRecord : public Record
{
private:
    MySql *m_dbConnection;

public:
    DatabaseRecord() : m_dbConnection(new MySql()) {}
    void start_record()
    {
        cout << "start transaction" << endl;
    }
    void store_field(
        const string &name,
        const string &value)
    {
        cout << "insert into table" << endl;
    }
    void finish_record()
    {
        cout << "finish transaction" << endl;
    }
};

int main()
{
    ContactData data =
        {
            "Black",
            "Smith",
            "439-234-422",
            "adam@email.com"};
    StreamRecord sRecord(std::cout);
    ContactRecorder contact(&sRecord);
    contact.store(data);

    DatabaseRecord dbRecord;
    ContactRecorder contact2(&dbRecord);
    contact.store(data);
    return 0;
}