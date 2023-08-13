/*
Basic Idea -------------------------
You have some objects
that will behave differently
depending on what state it is. (상태에 따라 다른 동작)

it doesn't matter 
how it got into that state ,
but depending on what state it is, 
it will do bunch of different things

ex) subway --> with subway card, we
walk through

what kind of state exist in
gate ?

1) Locked
2) Open
and we want to change state
between these 2 states

Now, what are the actions
that cause these transitions ?

payOk        : locked --> open
payFail      : locked --> locked( stay )
(user) enter : open --> locked

(paied but not entered yet,
try to put your card on the card 
machine again even if you paied)
payOk        : open --> open


We have 3 unique Actions, 2 unique State
        Action
State   
        payOk, payFail, enter
locked
open 
processing

----------------------------
Not using state pattern can quickly
become difficult to reason about 
- by using state pattern, we can 
divide all the possible cases, 
an assign,make appropriate
variable,functions 

By using state pattern ,
we can divide the states
and give them a name 
ex) In this state, I receive action,
what is the logical thing to do 

--------------------------------------
>> Definition
- state pattern allows an object
to alter its behavior
when its internal state changes .
- the object will appear to 
change its class 
- when its internal state changes ,
behaviour of different methods 
ex) payFail, enter ...
should change 

ex) when state is Open, Closed
the "payOk" method behaves 
in different way

-------------------------------------
>> Diagram

// "Context" have the "State"
ex) Gate has many different states  
- open, closed , processing
ex) class Gate{
    // different messages, behaviors
    // we can send to gate .
    // below methods somehow need to
    // delegate down to the gate state
    GateState state;
public : 
    // give the reference of its own to "concrete state"
    // so that "concrete state" which "changes state" 
    // can use "changeState(GateState gs)" method 
    Gate(){this.state=new ClosedGateState(this);}
    void enter() --> call GateState.enter() == delegation
    {
        // delegate
        this.state.enter()
    }
    void pay()
    {
        this.state.pay()
    }
    void payOk()
    {
        this.state.payOk()
    }
    void payFailed()
    {
        this.state.payFailed()
    }
    void changeState(GateState gs)
    {
        state = gs;
    }
}

- whoever is using this particular
turnstile is "not interacting directly"
with the State
- that is why definition talks about
"internal" state 
- therefore, what somebody would use
is that they would use the context ,
which is "turnstile" 

--> request() : state.handle()
whenever you request "context" to do sth,
"context" itself is not capable of
handling that request ,
what it will do is that it will 
pass down the message, it will "delegate
down" by calling 'state.handle()'

and "context" cannot have an instance of
an interface , but it can have an
instance of any of these concrete
classes that happen to implement
"state" interface 

"context" does not necessarily know
exactly which state it has at a runtime
it just calls a "handle" method .
and the "concrete state" can do 
whatever it needs to do depending on
the state it's currently in. 
and "concrete states" will have
different implementations of method 

// "State" : Interface --> we want to have
multiple different states, but we want
to treat them uniformly 
--> handle()
ex) class GateState{ --> just an interface
    void enter()=0;
    void pay()=0;
    void payOk()=0;
    void payFailed()=0;
}

// (Concrete + Multiple) State : Implementation
--> handle()

- below "concrete states" can call
"changeState(GateState) methods" and
passing a changed or new state , so
they would produce a new state 
ex) class OpenedGateState{
    Gate gate;
public :
    OpenedGateState(Gate g){this.gate =g;}
    void enter()
    void pay()
    void payOk()
    {
        // we know 
        // 1) gate is open
        // 2) payment is ok
        // we don't need any 
        // if,else if, else ... 
        // this.gate --> returning back to "gate" state
        // that has "OpenedGateState" class
        this.gate.changedState(new ClosedGateState)
    }
    void payFailed()
}
ex) class ClosedGateState{
    enter()
    pay()
    payOk()
    payFailed()
}


*/