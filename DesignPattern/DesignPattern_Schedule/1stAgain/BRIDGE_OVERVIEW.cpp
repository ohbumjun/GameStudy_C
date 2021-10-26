/*
>> Definition --------------------------
- intent of bridge pattern is to 
decouple an abstraction from its
implementation, 
- so that two can vary independently
ex) abstration : Animal
ex) implementation : Cat

>> Explanation ------------------------
class IAbstractionA{ // interface
    speak()=0;
    move()=0;
}
class ImpleA : public IAbstractionA{
    speak(){}
    move(){}
}
class IAbstractionB{ // interface
    // That is, one Abstraction
    // has another Abstraction
    // this is why it is called
    // "Bridge" Pattern
    IAbstractionA ~; 
    speak()=0;
    move()=0;
}
class ImpleB : public IAbstractionB{
    ImpleA ~ ;
    speak(){}
    move(){}
}

Based on above structure,  ----------
we can rename by

// has 'IAbstractionA'
IAbstractionB == Abstraction
ImpleB == Concrete Abstraction

IAbstractionA == Implementor
ImpleA == Concrete Implementor 

What we want is 
we want to pair up
"any" of Concrete Abstraction 
with
"any" of Concrete Implementor

"any" thing from Concrete Abstraction
can be composed with
"any" thing from Concrete Implementor
ex) {A,B,C}
ex) {1,2,3}
-->{A,1}.....{C,3} : 9 classes
we don't have to build 9 classes
with 3 + 3 = 6 classes, + "bridge" pattern
we can produce "9 seperate function classes"

>> Final Diagram --------------------------

class Implementor{ // interface
    speak()=0;
    move()=0;
}
class Concrete Implementor : public Implementor{
    speak(){}
    move(){}
}
class IAbstractionB{ 
    // interface or abstract class
    // (if want to implement specific code here)
    // That is, one Abstraction
    // has another Abstraction
    // this is why it is called
    // "Bridge" Pattern
    IAbstractionA ~; 
    speak()=0;
    move()=0;
}
class ImpleB : public IAbstractionB{
    ImpleA; ~ // has "Concrete Implementor"
    speak(){}
    move(){}
}

Okay. we are building an webpage
ex) Spotify

we have View class
ex) long description view,
middle description view,
short or simple view

and we have Resource class
ex) music, photos, etc

class IResource // Implementor 
{
    string snippet();
    string image();
    string title();
    string url();
}
class ArtistResource : public IResource {
    Artist artist;
public :
    ArtistResource(Artist a){
        artist = a;
    }
    string override snippet()
    {
        return this.artist.biograph();
    }
    string override image()
    {
        return this.artist.image();
    }
    string override title()
    {
        return this.artist.title();
    }
    stirng override url()
    {
        return this.artist.url();
    }
}
class MusicResource : public IResource {}

(abstract) class View // IAbstractionB
{
    IResource resource;
    public :
        View (IResource rsc)
        {this.resource = rsc;}
        string show()=0;
}
class LongFormView : public View {
    public override string show()
    {
        //...
        // LongForm would access "Resources"
        // ...
        this.resource.snippet();
        return html;
    }
    
}
coass MiddleFormView : public View {}


>> Recap of Definition
to 
decouple an abstraction from its
implementation, 
- so that two can vary independently

So...it seems like
we have 2 abstract-->concrete Hierarchy.
one Hierearchy "has" another "Heirarchy"
and we can compose from 2 concrete classes;

On the one hand, we have
"Abstraction", & "Implementation"
"Concrete Abstraction" &
"Concrete Implementation"

and "Abstraction" has "Implementation"

---------------------------------------------------
https://www.crocus.co.kr/1537

구현(implementation)으로부터
추상(abstration) 레이어를 분리하영
이 둘이 서로 독립적으로
변화할 수 있도록 한다.

구현부에서, 추상층을 분리하여
각자 독립적으로 변형이 가능하고
확장이 가능하도록 한다.

즉, 기능과 구현에 대해서
2개를 별도의 클래스로 구현을 한다.

// 상호 독립적인 Update(변경,확장,추가)를 편리하게 한다.
// 구현부의 변화없이, 인터페이스를 변경하고
// 인터페이스의 변화없이 구현부를 변경할 수 있다
// 결국 계층의 추가를 이용한 유연성의 증대

*/