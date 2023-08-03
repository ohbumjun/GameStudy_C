#ifndef __EvaVM_h

#define __EvaVM_h

#include "../bytecode/OpCode.h"

#include <vector>
#include <string>

class EvaVM
{
public :
    EvaVM(){};

    void exec(const std::string& program)
    {
        // 1. parse program to ast
        // auto ast = parser->parse(program)

        // 2. compile program to Eva bytecode
        // code = compiler->compile(ast)
        code = {OP_HALT};

        // Set instruction pointer to first byte of bytecode (혹은 program counter 라고도 불린다.)
        ip = &code[0];

        return eval();
    }

    /*
    Main eval loop
    */
   void eval()
   {
    for (;;)
    {
        switch(READ_BYTE())
    }
   }

    /*
    Intruction Pointer / Program Counter
    */
   uint8_t* ip;

    /*
    ByteCode == array of bytes
    */
    std::vector<uint8_t> code;
};

#endif