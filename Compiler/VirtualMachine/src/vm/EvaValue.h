#ifndef EvaValue_h
#define EvaValue_h

enum class EvaValueType
{
    NUMBER,
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
    };
};

/*
Constructors
*/

// EvaValue
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, .number = value})

// Address EvaValue as plain number
#define AS_NUMBER(evaValue) ((double)(evaValue).number)

#endif