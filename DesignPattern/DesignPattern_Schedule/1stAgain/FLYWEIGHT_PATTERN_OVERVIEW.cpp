/*
목표 : 메모리 공간을 절약한다.
- 즉, 메모리를 가볍게 유지
- 키워드 : 메모리 !

즉, 데이터가
프로그램하는 과정에서
작거나 한 것들은,

그때그때 생성하지 말고
한꺼번에 끄집어서 사용하자

-------------------------------------

즉, 프로그램 짤 때
중요하지 않지만, 필요한 것들

너무 크고 방대하면
db를 구축하고 사용해야 하지만,

그때그때 필요할 때마다
잠깐 사용하고 버릴 것들.
ex) 소모품

그것들을 보관했다가
뽑아쓰자.

ex) 인공지능 로봇
-- 기능 : a,b,c,d,e -> 가벼운 기능

< 정리 >
- 많은 유사한 객체를 공유해서 비용을 
절감할 수 잇다
- 대량의 작은 객체들을 공유하는 패턴이다
- 자주 사용하는 동일한 데이터나 코드를
중복 생성하지 않도록 관리하는
저장소( ex) map )를 만들어
필요할 때마다
불러 사용할 수 있도록 한다.

----------------------------------------


*/
#include <iostream>
#include <string>
#include <map>

using namespace std;

// FlyWeight
// 데이터 저장, 데이터 반환 기능
class FlyWeight
{
private:
    string m_Data;

public:
    FlyWeight(string data)
    {
        m_Data = data;
    }

public:
    string getData()
    {
        return m_Data;
    }
};

// FlyWeight 객체를 보관하고 관리하는 객체를 따로 둔다
// 보통,  factory 등을 마련한다.

class FlyWeightFactory
{
    map<string, FlyWeight *> DataPool;

public:
    string getFlyWeight(
        string key,
        string data)
    {
        FlyWeight *fly = DataPool[key];
        // 없으면 생성해서 반환
        if (!fly)
        {
            fly = new FlyWeight(data);
            DataPool[key] = fly;
            cout << "데이터 삽입" << endl;
            return fly->getData();
        }
        cout << "데이터 사용" << endl;
        return fly->getData();
    }
};

int main()
{
    FlyWeightFactory ff;
    string fw;
    fw = ff.getFlyWeight("123", "홍길동");
    cout << fw << endl;
    fw = ff.getFlyWeight("123", "홍길동");
    cout << fw << endl;
}