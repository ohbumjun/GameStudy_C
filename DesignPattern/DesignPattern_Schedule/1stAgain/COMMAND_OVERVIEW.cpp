/*
핵심 키워드
- encapsulate
- collections of commands
- no parameter
- Invoker, Command Interface


---------------------------------------------------
>> Basic Idea
-->> "command pattern encapsulate a request as an object"
- you have an object, and somebody sends a requests 
to that object to do sth, 
and request is what we're trying to encapsulate 
- we're not about to encapsulate the object
receiving the request , or object sending the request 
- we' re about to encapsulate the actual request , actual
command itself 

-->> "thereby letting you parameterize other objects with
different requests queue or log requests ""
- and then we can take a bunch of these encapsulated
commands and compose them into some context 
- we encapsulate multiple commands as objects , 
and you can pass these objects around 
- you can now have things that are sets of command ,
so you can have list of commands you are about to
execute , you can have collections of commands
that you execute when you press different buttons 
- queue ? it seems like a list. so you take a bunch
of commands and you put them in a list , 
you invoke the commands one by one , 
you have queue of commands you execute 

-->> "and support undoable operations "
- you have a command that does something.
the Inverse of that command is an undo. 
ex) command that adds one --> inverse : undo definition of that 
command, which is minus one 
- If I have a command that does something, it also supports
an undo which is the inverse of itself, then I can very 
simpley call the command and undo the operations by calling
undo on that command 
- If you have bunch of different commands, that have
encapsulated different actions and all of them follow 
this logic, that all of them have these undo operations ,
it's very simple to implement very complex undo operations,
because all of the different commands are responsible
for undoing themselves. 

>> Process ----------------------------------------------
- thing that invoke the command
ex) Light remote controller 
- we want to press button
- we want to send the command to a particular
device that does particular thing

That is ,
- attaching the command to a button
- sending the command whenever you press the button 

That is,
- we construct the command
- we inject the command into invoker
- whenever the invoker is pressed, we
execute the command 
- the command does sth to a receiver 

>> Diagram ---------------------------------------------
- Invoker ==> has 0 to many "Command" Interface ( ICommand )
but not concrete commands, it couples to 
interface I command 

class Invoker
{
    // keep track of commands 
    
    // invoker should be instantiated with 
    // 4 commands in this case  
    Invoker(ICommand on,ICommand off,ICommand up,ICommand down)
    {this.on=on;this.off=off;this.up=up;this.down=down;}
    
    // since Invoker has Command INstance variables
    // it means, it has the capability of turning the
    // lgith on ,off ... etc 
    // using the variables below 
    ICommand on;// represents 'on' button
    ICommand off; 
    ICommand up;
    ICommand down;

    public :
        void clickOn(){this.on.execute();}
        void clickOff(){this.off.execute();}

}

class ICommand  --> Interface : sth that needs to be shared
{
    // no parameter !
    // -- why ? command itself encapsulated
    // all of the things that need to perform
    // -- there is an action encapsulated in a
    // command , the command doesn't require
    // you to pass anything to it when it wants
    // to be executed 
    // you've already passed all of its dependencies
    // to it upon instantiation 
    void execute()
    void unexecute()
}

- Interface ICommand has many implementations
ex) turn on light command
ex) turn off light command 

class LightOnCommand : public ICommand
{
    public :
    // command has to know about all the 
    // dependencies it needs to perform its jobs
    // ex) need access to receiver --> in this case, light !
        Light light;
        Command(Light L){
            this.light = L;
        }
        void execute(){
            // acts upon its receiver
            this.light.on();
        }
        void unexecute(){this.light.off();}
}

- Any particular command acts upon a receiver
class Receiver
{
    ~~
} 

int main()
{
    Invoker *Iv = new Invoker(new LightOnCommand(light), ~~)
}

--------------------------------------------------------


*/