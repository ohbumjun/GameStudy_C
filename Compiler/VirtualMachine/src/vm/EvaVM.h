#ifndef __EvaVM_h

#define __EvaVM_h

#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "../parser/EvaParser.h"
#include "../compiler/EvaCompiler.h"
#include "./EvaValue.h"

#include <vector>
#include <string>
#include <array>

using syntax::EvaParser;

/*
Stack Overflow (after excedding)
*/
#define STACK_LIMIT 512

/*
Binary Operation
*/
#define BINARY_OP(op) \
    do{ \
        auto op2 = AS_NUMBER(pop()); \
        auto op1 = AS_NUMBER(pop()); \
        push(NUMBER(op1 op op2)); \
    }while(false)

/*
*  Read Byte from instruction Pointer 
    & increase instruction Pointer to indicate next byte
*/
class EvaVM
{
#define READ_BYTE() *ip++

public :
    EvaVM() : 
    parser(std::make_unique<EvaParser>()),
    compiler(std::make_unique<EvaCompiler>())
    {
        sp = stack.data();
    };

    EvaValue exec(const std::string& program)
    {
        // 1. parse program to ast
        auto ast = parser->parse(program);

        // 2. compile program to Eva bytecode
        // compiler : accepts ast => produce bytecode & associated data structure (ex. constant pool)
        co = compiler->compile(ast);
        
        // ex) (+ "Hello", "World") => code = {OP_CONST, 0,OP_CONST,1,OP_ADD,OP_HALT}
        // constants.push_back(ALLOC_STRING("Hello"));
        // constants.push_back(ALLOC_STRING(" World!"));
        // code = {OP_CONST, 0, // constant pool indexOP_CONST,1,OP_ADD,OP_HALT};

        // Set instruction pointer to first byte of bytecode (혹은 program counter 라고도 불린다.)
        ip = &co->code[0];

        return eval();
    }

    /*
    Main eval loop
    - bytecode 를 읽고나서 처리하기
    */
   EvaValue eval()
   {
        for (;;)
        {
            // 
            auto opcode = READ_BYTE();
            
            // Print the byte value in hexadecimal format
            uint8_t byteValue = static_cast<uint8_t>(opcode);
            std::cout << "opcode: " << std::hex << std::setw(2) << std::setfill('0') << +byteValue << "\n";
            
            switch(opcode)
            {
                case OP_HALT :
                    return pop();

                // --- constants ---
                case OP_CONST :
                    push(get_const());
                    break;
                case OP_ADD :
                    {
                    auto op2 = pop();
                    auto op1 = pop();

                    if (IS_NUMBER(op1) && IS_NUMBER(op2))
                    {
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);
                        push(NUMBER(v1 + v2));
                    }
                    else if (IS_STRING(op1) && IS_STRING(op2))
                    {
                        auto s1 = AS_CPPSTRING(op1);
                        auto s2 = AS_CPPSTRING(op2);
                        push(ALLOC_STRING(s1 + s2));
                    }
                    break;
                    }
                case OP_SUB :
                    {
                    BINARY_OP(-);
                    break;
                    }
                case OP_MUL :
                    {
                    BINARY_OP(*);
                    break;
                    }
                case OP_DIV :
                    {
                    BINARY_OP(/);
                    break;
                    }
                default :
                    DIE << "Unknown opcode : " << std::hex << opcode;
            }
        }
   }

   /*
   * push value on top of stack
   */
    void push(const EvaValue& value)
    {
        if ((size_t)(sp - stack.begin()) == STACK_LIMIT)
        {
            DIE << "push() : stack overflow.\n";
        }
        *sp = value;
        sp++;
    }

    EvaValue pop()
    {
        if (stack.size() == 0)
        {
            DIE << "pop() : empty stack";
        }
        --sp;
        return *sp;
    }

private :

    EvaValue get_const()
    {
        size_t constantIndex = READ_BYTE();
        return co->constants[constantIndex];
    }

    /*
    * Parser Instance
    */
    std::unique_ptr<EvaParser> parser;

    /*
    * Compiler Insatnce
    */
   std::unique_ptr<EvaCompiler> compiler;

    /*
    Intruction Pointer / Program Counter
    */
   uint8_t* ip;

   /*
   * stack Pointer : stack 내 EvaValue 를 가리킨다.
   */
    EvaValue* sp;

    /*
    Operand stack
    */
    std::array<EvaValue, STACK_LIMIT> stack;

    /*
    *  Code Objects
    */
   CodeObject* co;
};

#endif