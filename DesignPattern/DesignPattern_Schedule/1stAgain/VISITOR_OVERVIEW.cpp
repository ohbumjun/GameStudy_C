/* ----------------------------------------
What is Visitor Design Pattern ?
- allows you to add methods to classes of
different types without much altering to
those classes
- you can make completely different methods
depending on the class used
- allows you to define external classes
that can extend other classes
without majorly editing them


>> ----------------------------------------
What we are going to do below is , 
we are going to apply different amount of
taxes to different kinds of Products

class Visitor ( Interface ) // Visitable을 방문
{
    virtual double visit(Liquor)=0;
    virtual double visit(Tobacco)=0;
    virtual double visit(Necessity)=0;
}
class TaxVisitor : public Visitor
{
    // returns tax for Liquor
    double visit(Liquor){}; 
    virtual double visit(Tobacco){};
    virtual double visit(Necessity){};
}
class Visitable (Interface) // Visitor를 받아들이는 부분
{
    virtual double accept(Visitor)=0; 
}
class Necessity : Visitable
{
    double accept(Visitor){return Visitor.visit(Necessity)}
}

int main()
{
    TaxVisitor taxCale = new TaxVisitor();
    Necessity milk = new Necessity(3.47);
    milk.accept(taxCale);
}

---------------------------------------------------
방문자 패턴을 이용하여

객체(class) 에서 처리(method)를 분리. 해서
사용할 수 있다. 

구조는 변하지 않으며, 기능만을 따로 
추가하거나 확장되어야 할 경우에
사용하는 패턴이다. 

즉, 기존에 있던 객체 구조를 변경할 필요 없이
새로운 동작을 추가할 수 있다. 

보통 컴포지트 패턴(Composite Pattern)과
연동되어 사용되는 경우가 많다. 

Element : 개체로부터 분리시킨 요소
Visitor : 개체로부터 분리된 Element가 방문하는 개체

2개 interface
1) visitor : visitable을 방문
2) visitable( Element ) : visitor을 받아들임 


1) https://devjino.tistory.com/51
2) 강좌 : Udemy 1

*/
