#ifndef EvaCompiler_h
#define EvaCompiler_h

#include "../parser/EvaParser.h"
#include "../vm/EvaValue.h"
#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"


        

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
    }while(false);                                      \

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
                DIE << "Symbol : Unimplemented";
                break;
            }
            case ExpType::LIST :
            {
                DIE << "List : Unimplemented";
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

    /*
    * Compile code object
    */
    CodeObject* co;
};

#endif