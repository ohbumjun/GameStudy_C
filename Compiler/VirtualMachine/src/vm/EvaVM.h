#ifndef __EvaVM_h

#define __EvaVM_h

#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "./EvaValue.h"

#include <vector>
#include <string>
#include <array>

/*
Stack Overflow (after excedding)
*/
#define STACK_LIMIT 512

/*
*  Read Byte from instruction Pointer 
    & increase instruction Pointer to indicate next byte
*/

class EvaVM
{
#define READ_BYTE() *ip++

public :
    EvaVM()
    {
        sp = stack.data();
    };

    EvaValue exec(const std::string& program)
    {
        // 1. parse program to ast
        // auto ast = parser->parse(program)

        // 2. compile program to Eva bytecode
        // code = compiler->compile(ast)
        constants.push_back(NUMBER(2));
        constants.push_back(NUMBER(3));

        // byte code 에는 42 라는 number 가 아니라
        // 해당 number 를 담은 constant 의, constant pool 상의 idx 를 넣는다.
        code = {OP_CONST, 
        0, 
        OP_CONST, 
        1,
        OP_ADD,
        OP_HALT};

        // Set instruction pointer to first byte of bytecode (혹은 program counter 라고도 불린다.)
        ip = &code[0];

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
                    auto op1 = AS_NUMBER(pop());
                    auto op2 = AS_NUMBER(pop());
                    auto result = op1 + op2;
                    push(NUMBER(result));
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
        return constants[constantIndex];
    }

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
    ByteCode == array of bytes
    */
    std::vector<uint8_t> code;

    /*
    Constant Pool
    */
   std::vector<EvaValue> constants;
};

#endif