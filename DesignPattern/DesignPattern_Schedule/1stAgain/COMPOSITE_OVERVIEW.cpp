/*
>> Intent ------------------------------
- Composite object is made up of
multiple object , made up of multiple
parts 

- making composite of objects, single object
both implement the same interface or 
be part of same inheritance ,
thus making it possible for us to 
treat them uniformly

>> Definition --------------------------
- composite pattern composes object
into tree structures to represent
part whole hierarchy.
- composite lets client treat 
individual object, and compositions
of an object "uniformly"

>> Diagram -----------------------------
class Component // Interface
{
    operation(){
        // function you want to apply
        // across composition of objects
        // and single object "uniformly"
    }
    Add()
    Remove()
}

class Leaf // single object
{
    // Leaf "is" Component
    operation(){}
    // if Leaf.Add() --> return "new Composite"
}
class Composite : public Component // composite object
{
    // Composite "has","is" a Component
    operation(){}
}

>> Example ---------------------------
class TodoList // class Component
{
    string getHtml(){}
}
class Todo : public TodoList  // Leaf
{
    string m_text;
    public :
        Todo(string text) : m_text(text){}
        void string getHtml(){}
}
class Todos : public TodoList // Composite 
{
    string title;
    vector<TodoList> Todos;
    public :
        Todos(
            string i_title,
            vector<TodoList> i_todos
            ) 
            : 
            title(i_title),
            Todos(i_todos)
            {}
        string getHtml()
        {
            string html = "<ul><li>";
            html += title;
            html += "</li><ul>"
            for(int i = 0 ; i < Todos.size(); i++)
                html += "<li>"
                // Recursion !
                // iterate object in composite
                html += Todos[i].getHtml();
                html += "</li>"
            html += "</ul>"
        }
}

*/