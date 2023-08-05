#ifndef EvaCompiler_h
#define EvaCompiler_h

#include "../parser/EvaParser.h"
#include "../vm/EvaValue.h"
#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"

#include <string>
#include <map>

/*
Emits bytecde & records constant pool, vars, etc
*/
class EvaCompiler
{
#define ALLOC_CONST(tester, converter, allocator, value)\
    do {                                                \
        for (auto i = 0; i < co->constants.size(); ++i) \
        {                                               \
            if (!tester(co->constants[i]))              \
            {                                           \
                continue;                               \
            }                                           \
            if (converter(co->constants[i]) == value)   \
            {                                           \
                return i;                               \
            }                                           \
        }                                               \
        co->constants.push_back(allocator(value));      \
    }while(false);      
    


public :
    EvaCompiler(){};

    /*
    * Main compile API
    */
   CodeObject* compile(const Exp& exp)
   {
        co = AS_CODE(ALLOC_CODE("main"));

        // Generate recursively from top-level (functions ?)
        gen(exp); 

        // explicit vm-stop marker
        emit(OP_HALT);

        return co;
   }

    // recursively travel ast 
    void gen(const Exp& exp)
    {
        switch(exp.type)
        {
            case ExpType::NUMBER :
            {
                // Generate code as number
                // - put const in constant pool
                // - set value at that constant
                // - generate bytecode as a result
                emit(OP_CONST);
                emit(numericConstIdx(exp.number));
                break;
            }
            case ExpType::STRING :
            {
                emit(OP_CONST);
                emit(stringConstIdx(exp.string));
                break;
            }
            case ExpType::SYMBOL :
            {
                // ex. true
                if (exp.string == "true" || exp.string == "false")
                {
                    emit(OP_CONST);
                    emit(booleanConstIdx(exp.string == "true" ? true : false));
                }
                else 
                {
                    // variables
                }
                break;
            }
            case ExpType::LIST :
            {
                // ex. (- (+ 15 0) 5)
                auto tag = exp.list[0];

                if (tag.type == ExpType::SYMBOL)
                {
                    // .bnf 파일에서 +,-,*,/ 등 사칙연산은 string 으로 분류하게 세팅
                    auto op = tag.string;

                    if (op == "+")
                    {
                        gen_binary_op(exp, OP_ADD);
                    }
                    else if (op == "*")
                    {
                        gen_binary_op(exp, OP_MUL);
                    }
                    else if (op == "/")
                    {
                        gen_binary_op(exp, OP_DIV);
                    }
                    else if (op == "-")
                    {
                        gen_binary_op(exp, OP_SUB);
                    }
                    // compare operations
                    // compareOps_.count(op) : op key 에 대한 value 개수
                    // ex. (> 5 10)                    0 아니면 1 (중복 key 허용 안하므로)
                    else if (compareOps_.count(op) != 0)
                    {
                        gen(exp.list[1]);
                        gen(exp.list[2]);
                        emit(OP_COMPARE);
                        emit(compareOps_[op]);
                    }
                }

                break;
            }
        }
    }

private :

    /*
    * Emit data to bytecode
    */
    void emit(uint8_t code)
    {
        co->code.push_back(code);
    }

    /*
    * Allocate a numeric constant
      - return Idx 
    */
    size_t numericConstIdx(double value)
    {
        ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);
        return co->constants.size() - 1;
    }

    size_t stringConstIdx(const std::string& value)
    {
        ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
        return co->constants.size() - 1;
    }

    size_t booleanConstIdx(bool value)
    {
        ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
        return co->constants.size() - 1;
    }

    // Generate Binary Opertion as bytecode format
    // ex. (+ 1 2) OP_CONST, OP_CONST, OP_ADD
    void gen_binary_op(const Exp& exp, uint8_t op)
    {
        do{
            gen(exp.list[1]);
            gen(exp.list[2]);
            emit(op);
        }while(false);   
    }

    /*
    * Compile code object
    */
    CodeObject* co;

    /*
    * Compare ops -> encode to number
    */
   std::map<std::string, uint8_t> compareOps_ = {
    {"<", 0}, {">", 1}, {"==", 2}, {"<=", 4}, {">=", 5}
   };
};

#endif