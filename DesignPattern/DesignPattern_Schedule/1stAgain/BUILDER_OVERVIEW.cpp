/*
1) Strategy 전략 패턴
- 내가 원하는 부분만 바꾸고 싶다.
- 캡슐화 + 실행시간에 교체

2) State 상태 패턴
- 객체 상태에 따라 동작이 바뀐다.
- eX) 게임 캐릭터 아이템 획득에 따른 변화

3) Builder 빌터 패턴
- 구축공정은 동일 ==> 일반화가 가능하다

-------------------------------------------------

> User 

> class Director

> class IDirector


아래와 같은 구조에서, 사용자가 "주문 및 order" 시에
IBuilder class가 주문제작을 해주는 개념이다. 

즉, Director는 '지시'만 , 
실제 공정은 'Builder'가 

그렇다면, IBuilder Interface는 굳이 왜 필요한 것일까 ?
이를 통해 "구축공정"을 약속할 수 있기 때문이다. 

class Director
{
    class IBuilder
    {

    }
}

----------------------------------------------------
>> 정의 

Builder Pattern 정의
= "복잡한 단계"를 거쳐야 생성되는 객체의 구현을, 
"서브 클래스"에게 넘겨주는 패턴
( 즉, 복잡한 객체를, 단순한 객체들을 이용하여 만들기)  

- 동일한 구축공정( Director ) 로 객체를 만들지만, 
각 공정에 따른, 표현이 달리지는 객체를 만들 때,
축구 게임에서 모든 캐릭터는 모자/유니폼/신발이 있지만
어떤 모자를 사용하는지는 달라진다.
- Same construction process can
create different representations.s

왜 BDP 를 사용 ?
- Because I want to create an Object 
with composing other complext
object
- I want to create those objects
step by step (Director will handle that)

< 정리 >
복합 객체의 생성 과정과 표현 방법을 분리하여, 
동일한 생성 절차에서 
서로 다른 표현 결과를 만들 수 있게 하는 패턴

쉽게 말해서,
하나의 객체를 생성할 때, 
1) 객체를 이루는 구성요소들을 분리하여, 
2) 객체의 세부 구성요소 클래스들을 
별도로 만들고
3) 그 구성요소들을 조합하여
하나의 객체를 만드는 것이다

자동차를 만든다고 한다면
자동차의 부품들을 모아서
하나의 자동차를 만드는 것 

*/