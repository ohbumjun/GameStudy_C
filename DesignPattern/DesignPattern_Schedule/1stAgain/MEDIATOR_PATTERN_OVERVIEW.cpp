/*
<중재자 패턴>
M:N 관계를 1:1 로 변경하기

즉, 복잡한 관계를
간단한 관계로 구현한다.

ex) 그룹채팅
- 그룹채팅 이전의 문자 메시지는,
user1,2,3,4 가 
각각 따로 관계를 가지면서
메세지를 주고 받았어야 한다.

하지만, 그룹채팅 이후
그룹채팅 하나.라는
매개체가 있음으로써
커뮤니케이션 비용을
절감하게 된다.

-------------------------------

< UML >
2개의 abstract class
1) Mediator 
- v : 같이 협력하는 객체들을 
담는 그릇
- f : 객체 추가 함수
- f : 


2) Collegue( 객체들 )
- v : 중재자 정보(중재자 정보를 알아야 한다)
- f : 어떤 중재자에 등록하는 함수
- f : 중재자에게 특정 일을 알려주는 함수
ex) 그룹채팅에 메시지 올리기 

정리하자면, 
중재자 패턴을 사용하면,
객체 간 통신은
중재자 객체 안에 함축된다.

객체들은 더 이상 다른 객체와
서로 직접 통신하지 않으며
대신 중재자를 통해 통신한다.

이를 통해, 통신 객체 간
의존성을 줄일 수 있으므로
결합도를 감소시킬 수 있다. 


*/