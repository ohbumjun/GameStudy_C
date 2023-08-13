/*
FlyWeight 패턴은,
해당 객체 전부를 저장해서 관리할 수도 있지만,
객체 간 공통부분이
공유의 대상이 될수도 있다.

다시 말해서
FlyWeight 패턴은
각 객체에 있는 데이터를 유지하는 대신에
"여러 객체간에 상태의 공통부분"을
공유하여,
더 많은 객체를
"가용 RAM크기"에 맞출 수 있는
구조 설계 패턴을 의미한다.

다시 말해서, "비용이 큰 자원"을
공통으로 사용하게 하여, 비용을 줄이는 패턴

각 객체에 데이터를 유지하게 되면
메모리에 데이터가 점차 쌓이고
결국 Ram의 크기를 넘어설 수 있다.

"비용이 큰 자원"
1) 중복 생성될 가능성이 높거나
2) 비용에 비해 사용빈도가 낮은 자원

-----------------------------------------------
>> 구현
1) FlyWeight될 class의 field를 두 부분으로 분리한다
- intrinsic state : 많은 개체에 중복된 data field
- extrinsic state : 각 object 에 고유한 context data field

- 선택적으로 factory class를 만들어
flyweight pool 관리

Flyweight object method를 호출하기 위해서는
client가 extrinsic state를 저장하거나 계산해야 한다.

즉, 정리하자면, 
같은 객체 안에서도, 중복되는 부분이 있고, 고유한 부분이 있다

그렇다면, 중복되는 부분은, 
FlyWeight 객체안에 넣어서 
공유하게 만들고
고유한 부분은, 별도로 분리하여

작용하게 하는것 ( 하다보면 알겠지 )

=====
헷갈리는 점이 있다.
Shared State의 중복생성자.

자. 여기서
map<string, FlyWeight>
--> FlyWeight 객체
    --> Shared State 객체

우리는 FlyWeight 객체를
공유해서 사용하고 있다

그리고 각각의 FlyWeight 객체의 경우, 
Shared State 라는 객체를 속성을 지닌다.

map에 대한 value로는 FlyWeight 자체를 가지고
그 안에 Shared State에 대한 포인터를 지닌다

굳이 설명하자면, 
원래
<string, Shared State*>로 되야 할것을

중간에 FlyWeight라는 객체로 한번 더 감싸고
FlyWeight라는 객체 안에서
Unique State 와의 상호작용이 
이루어나도록 한 것 같다. 

그리고 Shared State*의 복사생성자는
맨 처음 FlyWeight 객체를 만들때
Shared State* 에 해당하는 value를 insert 하는데
이때 값을 세팅해주기 위한 것이다 

*/
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;

struct SharedState
{
    string brand_;
    string model_;
    string color_;

    SharedState(
        const string &brand,
        const string &model,
        const string &color) : brand_(brand),
                               model_(model),
                               color_(color)
    {
        // cout << "대입 생성자" << endl;
    }

    // 복사 생성자
    SharedState(const SharedState &shared_state)
    {
        // cout << "복사 생성자" << endl;
        // 얕은 복사
        *this = shared_state;
    }

    friend ostream &operator<<(ostream &os, const SharedState &ss)
    {
        return os << "[" << ss.brand_ << "," << ss.model_ << "," << ss.color_ << "]";
    }
};

struct UniqueState
{
    string owner_;
    UniqueState(
        const string &owner) : owner_(owner)
    {
    }
    friend ostream &operator<<(
        ostream &os,
        const UniqueState &us)
    {
        return os << "[" << us.owner_ << "]";
    }
};

class FlyWeight
{
private:
    SharedState *shared_state_;

public:
    FlyWeight(
        const SharedState *shared_state)
    {
        shared_state_ = new SharedState(*shared_state);
    }
    FlyWeight(const FlyWeight &other)
    {
        shared_state_ = new SharedState(*other.shared_state_);
    }
    ~FlyWeight() { delete shared_state_; }

    // 형변환 연산자
    SharedState *shared_state() const
    {
        return shared_state_;
    }
    void Operator(const UniqueState &unique_state) const
    {
        cout << "FlyWeight displaying shared (" << *shared_state_ << ") and unique " << unique_state << ") state";
    }
};

class FlyWeightFactory
{
private:
    unordered_map<string, FlyWeight> flyweights_;
    string GetKey(const SharedState &ss) const
    {
        return ss.brand_ + "-" + ss.model_ + "-" + ss.color_;
    }

public:
    FlyWeightFactory(initializer_list<SharedState> shared_states)
    {
        for (const SharedState &ss : shared_states)
        {
            flyweights_.insert(
                std::make_pair<string, FlyWeight>(GetKey(ss), FlyWeight(&ss)));
        }
    }
    FlyWeight GetFlyWeight(
        const SharedState &shared_state)
    {
        string key = GetKey(shared_state);
        if (flyweights_.find(key) == flyweights_.end())
        {
            cout << "FlyWeightFactory : Can't find a flyweight, creating new one" << endl;
            flyweights_.insert(
                std::make_pair(key, FlyWeight(&shared_state)));
        }
        else
        {
            cout << "FlyweightFactory : Reusing existing flyweight";
        }
        // returns reference to mapped value of the element
        return flyweights_.at(key);
    }
    void ListFlyWeights() const
    {
        size_t count = flyweights_.size();
        cout << "FlyWeightFactory :  I have"
             << count << " flyweights" << endl;
        for (std::pair<string, FlyWeight> pair : flyweights_)
            cout << pair.first << endl;
    }
};

void AddCarToPoliceDatabase(FlyWeightFactory *ff,
                            const string &owner,
                            const string &brand,
                            const string &model,
                            const string &color)
{
    cout << "\n Client adding a car to database" << endl;
    const FlyWeight &flyweight = ff->GetFlyWeight({brand, model, color});

    flyweight.Operator({owner});
}

int main()
{
    FlyWeightFactory *factory = new FlyWeightFactory({{"Chevrolet", "Camaro2018", "pink"},
                                                      {"Mercedes Benz", "C300", "black"},
                                                      {"Mercedes Benz", "C500", "red"},
                                                      {"BMW", "M5", "red"},
                                                      {"BMW", "X6", "white"}});
    factory->ListFlyWeights();

    AddCarToPoliceDatabase(
        factory,
        "Bj",
        "BMV",
        "M5",
        "red");

    AddCarToPoliceDatabase(
        factory,
        "Yoona",
        "James Doe",
        "X1",
        "red");

    factory->ListFlyWeights();
    delete factory;

    return 0;
}