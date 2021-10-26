/*
What is Memento Design Pattern ?
- way to store previous state of Object easily
>  Memento : the basic object that is stored in
different states

>  Originator : Sets & Gets values from
currently targeted Memento. Creates new Mementos
and assigns current values to them
- protects against access by objects 
other than the originator

> Caretaker : holds an arraylist that contains
all previous versions of the Memento.
It can store and retrieve stored Mementos

class Memento
{
    string article;
    Memento(string ss) : article(ss);
    string getArticle(){}
}
class Originator
{
    string article;
    void set(string)
    Memento createMemento(){}
    string restoreMemento(){}
}
class CareTaker{
    vector<Memento*>mementos;
    void addMemento{}
    Memento* getMemento(){}
}

---------------------------------------------------
" 메멘토 : 기념품,유품,추억거리 "" 

Originator : 우리가 다루는 객체. 원본의 상태를 가진 객체
Memento : Originator의 상태를 저장하는 객체
CareTaker : Memento 관리해주는 관리자 역할 

메멘토 패턴은, 개체의 상태를 기록해 놓았다가
원래 상태로 복원할 필요가 있을 때
기록해 놓은 것을 사용하는 패턴.

메멘토 패턴은, 실행 취소 기능을 
지원하고자 할 때 많이 사용한다.

어떠한 응용에서 특정 기능을 수행할 것을 
요청하기 전에, 원본 개체에게 메멘토 개체를 요청한다.

원본 개체는, 자신의 상태 정보에 대한
스냅샷인 메멘토 개체를 생성하여 반환한다.

(즉, 메멘토는, 현상태의 스냅샷 개념)

그리고, 특정 기능을 수행하다가, 수행 이전 
상태로 복원이 필요하게 되면, 
반환 받았던 메멘토 개체를
원본 개체에게 전달하여
복원할 것을 요청한다.

원본 개체는, 메멘토 개체에
보관해두었던 이전 상태 값을 얻어와서
자신의 상태를 복원하는 것이다. 

*/