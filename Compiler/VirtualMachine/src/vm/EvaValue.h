#ifndef EvaValue_h
#define EvaValue_h

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

class Object;

enum class EvaValueType
{
    NUMBER,
    OBJECT,
    BOOLEAN,
};

struct LocalVar
{
    std::string name;
    size_t scopeLevel;
};

/*
Eva value {tagged union}
- low level 단에서 number 를 어떻게 처리할까 ?
- c/c++ 의 경우 pointer 도 low 단에서는 그저 숫자에 불과하다
- 따라서 low level 단에서는 value + type 형태의 struct 로 처리한다.
*/

struct EvaValue
{
    EvaValueType type;
    union 
    {
        double number;

        bool boolean;
        
        // ex. string , code
        Object* object;
    };
};

enum class ObjectType
{
   STRING,
   CODE,   // EvaCompiler 가 만들어내는 Code
   NATIVE  // function
};

struct Object
{
    Object(ObjectType type) : type(type){}
    ObjectType type;
};

struct StringObject:public Object{
    StringObject(const std::string& str) : Object(ObjectType::STRING), string(str){}
    std::string string;
};

// Function ---

using NativeFn = std::function<void()>;

struct NativeObject : public Object
{
    NativeFn func;

    std::string name;

    // 해당 함수가 끝나고 stack 으로부터 pop 해야할 지역변수 및 매개변수 개수
    size_t arity;

    NativeObject(NativeFn fn, const std::string& name, size_t arity) :Object(ObjectType::NATIVE),
        func(fn),
        name(name),
        arity(arity){}
};
// -----------

struct CodeObject:public Object{
    CodeObject(const std::string& str) : Object(ObjectType::CODE), name(str){}
    
    std::string name;

    // constant pool
    std::vector<EvaValue> constants;

    // byte code
    std::vector<uint8_t> code;

    // current scope level -> nested block 이 생길 때마다, scopeLevel 1 증가
    size_t scopeLevel = 0;

    // local variables and functions
    // - Local Block 에서 Local variable 은 stack 에 쌓인다. vector 를 stack 형태로 사용하고자 getLocalIndex 는 맨 뒤에 있는 요소 부터 찾고자 하는 것이다.
    std::vector<LocalVar> locals;

    void addLocal(const std::string& name)
    {
        locals.push_back({name, scopeLevel});
    }

    // get global index
    int getLocalIndex(const std::string& name)
    {
        if (locals.size() == 0) return -1;

        for (auto i = (int)locals.size() - 1; i >= 0; --i)
        {
            if (locals[i].name == name)
            {
                return i;
            }
        }

        return -1;
    }
};

/*
Constructors
*/

// EvaValue
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})
#define BOOLEAN(value) ((EvaValue){EvaValueType::BOOLEAN, .boolean = value})


#define ALLOC_STRING(value) ((EvaValue){EvaValueType::OBJECT, .object = (Object*)new StringObject(value)})
#define ALLOC_CODE(name) ((EvaValue){EvaValueType::OBJECT, .object = (Object*)new CodeObject(name)})
#define ALLOC_NATIVE(fn, name, arity)   \
 ((EvaValue){EvaValueType::OBJECT,     \
 .object = (Object*)new NativeObject(fn, name, arity)})

// Address EvaValue as plain number
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_BOOLEAN(evaValue) ((bool)(evaValue).boolean)
#define AS_STRING(evaValue) ((StringObject*)(evaValue).object)
#define AS_CODE(evaValue) ((CodeObject*)(evaValue).object)
#define AS_NATIVE(evaValue) ((NativeObject*)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)
#define AS_OBJECT(evaValue) ((evaValue).object)

// Testers
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_BOOLEAN(evaValue) ((evaValue).type == EvaValueType::BOOLEAN)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)
#define IS_OBJECT_TYPE(evaValue, objectType) \
    (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType)
#define IS_STRING(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::STRING)
#define IS_CODE(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::CODE)
#define IS_NATIVE(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::NATIVE)

std::string evaValueToTypeString(const EvaValue& evaValue)
{
    if (IS_NUMBER(evaValue))return "NUMBER";
    else if (IS_BOOLEAN(evaValue))return "BOOL";
    else if (IS_STRING(evaValue))return "STRING";
    else if (IS_CODE(evaValue))return "CODE";
    else if (IS_NATIVE(evaValue))return "NATIVE";
    else 
    {
        DIE << "evaValueToTypeString : unknown type " << (int)evaValue.type ;
    }
    return ""; // unreachable
}

std::string evaValueToConstantString(const EvaValue& evaValue)
{
    std::stringstream ss;
    if (IS_NUMBER(evaValue))
        ss << evaValue.number;
    else if (IS_BOOLEAN(evaValue))
        ss << (evaValue.boolean ? "true" : "false");
    else if (IS_STRING(evaValue))
        ss << '"' <<  AS_CPPSTRING(evaValue) << '"';
    else if (IS_CODE(evaValue))
    {
        auto code = AS_CODE(evaValue);
        ss << "code" << code << ": " << code->name;
    }
    else if (IS_NATIVE(evaValue))
    {
        auto fnObj = AS_NATIVE(evaValue);
        ss << "code " << fnObj->name << " / arity : " << fnObj->arity;
    }
    else 
    {
        DIE << "evaValueToConstantString : unknown type " << (int)evaValue.type ;
    }

    return ss.str();
}

// log(string type) 이 동작하도록 하기 위함
std::ostream &operator << (std::ostream& os, const EvaValue& evaValue)
{
    return os << "EvaValue (" << evaValueToTypeString(evaValue)
            << ") " << evaValueToConstantString(evaValue);
}

#endif