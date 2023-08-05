#ifndef EvaValue_h
#define EvaValue_h

#include <string>

enum class EvaValueType
{
    NUMBER,
    OBJECT
};

enum class ObjectType
{
   STRING
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
        
        // ex. string
        Object* object;
    };
};

/*
Constructors
*/

// EvaValue
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})

#define ALLOC_STRING(value) ((EvaValue){EvaValueType::OBJECT, .object = (Object*)new StringObject(value)})

// Address EvaValue as plain number
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_STRING(evaValue) ((StringObject*)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)
#define AS_OBJECT(evaValue) ((evaValue).object)

// Testers
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)
#define IS_OBJECT_TYPE(evaValue, objectType) \
    (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType)
#define IS_STRING(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::STRING)

#endif