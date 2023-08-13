/*
Instruction set for eva VM

- (참고 : bytecode 형태로 code 가 변환되고, 그 bytecode 로 코드를 실행하는 것이라면
          byte 별로 의미가 다 다르게 다르다는 것이고, 어떤 byte 가 어떤 의미인지에 대한
          대응관계가 존재하는 것인가? )
*/
#ifndef __OpCode_h

#define __OpCode_h

#include <string>
#include <cstdint>
#include "../utils/Logger.h"

/*
Stop Program
- Prograrm 을 중지하라는 의미의 bytecode 는 0 이다.
*/
#define OP_HALT 0x00

/*
Push a const onto stack
*/
#define OP_CONST 0x01

/*
Math operation
*/
#define OP_ADD 0x02
#define OP_SUB 0x03
#define OP_MUL 0x04
#define OP_DIV 0x05

/*
Compoare operation
*/
#define OP_COMPARE 0x06

/*
Control Flows
*/
// jump to alternate if condition is false
#define OP_JMP_IF_FALSE 0x07

#define OP_JMP 0x08

/*
Return global variable
*/
#define OP_GET_GLOBAL 0x09

#define OP_SET_GLOBAL 0x10

/*
Pop value from stack
*/
#define OP_POP 0x11

/*
Return local variable
*/
#define OP_GET_LOCAL 0x12

#define OP_SET_LOCAL 0x13

/*
Exit Scope
*/
#define OP_SCOPE_EXIT 0x14

/*
Call function
*/
#define OP_CALL 0x15

/*
Return from function
*/
#define OP_RETURN 0x16

#define OP_STR(op)  \
    case OP_##op :  \
        return #op; \

std::string opcodeToString(uint8_t opcode)
{
    switch (opcode)
    {
        OP_STR(HALT);
        OP_STR(CONST);
        OP_STR(ADD);
        OP_STR(SUB);
        OP_STR(MUL);
        OP_STR(DIV);
        OP_STR(COMPARE);
        OP_STR(JMP_IF_FALSE);
        OP_STR(JMP);
        OP_STR(GET_GLOBAL);
        OP_STR(SET_GLOBAL);
        OP_STR(POP);
        OP_STR(GET_LOCAL);
        OP_STR(SET_LOCAL);
        OP_STR(SCOPE_EXIT);
        OP_STR(CALL);
        OP_STR(RETURN);
        default :
        {
            DIE << "opcodeToString : unknown opcode : " << std::hex << (int)opcode; 
        }

        return "Unknown";
    };

    return "";
};

#endif