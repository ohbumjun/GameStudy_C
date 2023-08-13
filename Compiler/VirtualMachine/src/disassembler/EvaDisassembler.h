#ifndef EvaDisassembler_h
#define EvaDisassembler_h

#include "../vm/EvaValue.h"
#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "../vm/Global.h"

/*
Eva disassembler
*/
class EvaDisassembler
{
public :

    EvaDisassembler(std::shared_ptr<Global> global) :
        global(global)
        {
            std::cout << "Disassembler constructor" << std::endl;
        }

    /*
    * ByteCode to Textural Expression
    */
    void disassemble(CodeObject* co)
    {
        std::cout << "--- Disassembly ---" << std::endl;
        std::cout << "Code Name : " << co->name << std::endl;

        size_t offset = 0;

        while(offset < co->code.size())
        {
            offset = disassembleInstruction(co, offset);
            std::cout << std::endl;
        }
    }

private :


    size_t disassembleInstruction(CodeObject* co, size_t offset)
    {
        std::ios_base::fmtflags f(std::cout.flags());

        // Print bytecode offset
        std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << offset << " ";

        auto opcode = co->code[offset];

        switch (opcode)
        {
            case OP_HALT :
            case OP_ADD  :
            case OP_MUL  :
            case OP_DIV  :
            case OP_SUB  :
            case OP_POP  :
            {
                // OP_HALT 는 1byte 에 해당하는 명령어 -> disassembleSimple 함수로 처리.
                return disassembleSimple(co, opcode, offset);
            }
            case OP_SCOPE_EXIT :
            case OP_CALL :
            {
                return disassembleWord(co, opcode, offset);
            }
            case OP_CONST :
            {
                return disassembleConst(co, opcode, offset);
            }
            case OP_COMPARE :
            {
                return disassembleCompare(co, opcode, offset);
            }
            case OP_JMP_IF_FALSE :
            case OP_JMP :
            {
                return disassembleJump(co, opcode, offset);
            }
            case OP_GET_GLOBAL :
            case OP_SET_GLOBAL :
            {
                return dissassebleGlobal(co, opcode, offset);
            }
            case OP_GET_LOCAL :
            case OP_SET_LOCAL :
            {
                return dissassebleLocal(co, opcode, offset);
            }
            default :
            {
                DIE << "no assembly in disassebleInstruction for " << opcodeToString(opcode) << std::endl;
                break;
            }
        }

        std::cout.flags(f);

        return 0;
    }

    size_t disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset)
    {
        // ex. Offset : 0000, Bytes : 00, Opcode : HALT
        dumpBytesToStringStream(co, offset, 1);
        printOpcode(opcode);
        return offset + 1;
    }
    
    size_t disassembleWord(CodeObject* co, uint8_t opcode, size_t offset)
    {   
        dumpBytesToStringStream(co, offset, 2);
        printOpcode(opcode);
        // ex. ON_SCOPE_EXIT : num of local vars to pop from stack
        // ex. ON_CALL       : num of fn argumnets
        std::cout << (int)co->code[offset + 1];
        return offset + 2;
    }

    size_t disassembleJump(CodeObject* co, uint8_t opcode, size_t offset)
    {
        // Usage : temporarily modify the formatting flags of std::cout and then restore them
        std::ios_base::fmtflags f(std::cout.flags());
        
        dumpBytesToStringStream(co, offset, 2);

        printOpcode(opcode);

        // 1. else 시작문
        // 2. 혹은 if 문이 이미 실행된 이후, else 를 뛰어넘은 이후 address
        uint16_t addressOffset = readWordAtOffset(co, offset + 1);

        std::cout << std::uppercase << std::hex << std::setfill('0') << 
            std::setw(4) << (int)addressOffset << " ";

        std::cout.flags(f);

        // <condition> <if> <else>
        return offset + 3;
    }

    uint16_t readWordAtOffset(CodeObject* co, size_t offset)
    {
        return (uint16_t)((co->code[offset] << 8) | co->code[offset + 1]);
    }

    size_t disassembleCompare(CodeObject* co, uint8_t opcode, size_t offset)
    {
        dumpBytesToStringStream(co, offset, 2);
        printOpcode(OP_COMPARE);

        // 비교연산자는, 그에 대응되는 int 형태로 encode 되어있는 상태.
        auto compareOp = co->code[offset + 1];

        std::cout << (int)compareOp << "(";
        std::cout << inverseCompareOps_[compareOp] << ")";
        return offset + 2;
    }

    size_t dissassebleLocal(CodeObject* co, uint8_t opcode, uint8_t offset)
    {
        // 2byte : opcode , global variable index
        dumpBytesToStringStream(co, offset, 2);
        printOpcode(opcode);

        auto localIndex = co->code[offset + 1];
        
        std::cout << (int)localIndex << " (" << co->locals[localIndex].name << ")";
    
        return offset + 2;
    }

    size_t dissassebleGlobal(CodeObject* co, uint8_t opcode, uint8_t offset)
    {
        // 2byte : opcode , global variable index
        dumpBytesToStringStream(co, offset, 2);
        printOpcode(opcode);

        auto globalIndex = co->code[offset + 1];
        
        std::cout << (int)globalIndex << " (" << global->get(globalIndex).name << ")";
    
        return offset + 2;
    }

    size_t disassembleConst(CodeObject* co, uint8_t opcode, uint8_t offset)
    {
        // ex. Offset : 0000, Bytes : 01 00, Opcode : CONST, Arg 0 (15)
        //     2bytes : [1byte = OP_CONST] [1byte = idx in constant pool]
        dumpBytesToStringStream(co, offset, 2);
        
        printOpcode(opcode);

        auto constIndex = co->code[offset + 1];

        // print Constant Number
        std::cout << "Constant Pool Idx " << (int)constIndex << " (" <<
            evaValueToConstantString(co->constants[constIndex]) << ")";
    
        return offset + 2;
    }

    /*
    * Dump stringstream raw memory from bytecode
        - print 'bytes' to 'string' format 
        - ex) Offset : 0000, Bytes : 01 00
    */
   void dumpBytesToStringStream(CodeObject* co, size_t offset, size_t count)
   {
        // Usage : temporarily modify the formatting flags of std::cout and then restore them
        std::ios_base::fmtflags f(std::cout.flags());
        
        std::stringstream ss;

        for (auto i = 0; i < count; ++i)
        {
            ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2) 
            << (((int)co->code[offset + i]) & 0xFF) << " ";
        }

        std::cout << std::left << std::setfill(' ') << std::setw(12) << ss.str();
        
        std::cout.flags(f);
   }

    /*
    * Opcode
    */
   void printOpcode(uint8_t opcode)
   {
        // Usage : temporarily modify the formatting flags of std::cout and then restore them
        std::ios_base::fmtflags f(std::cout.flags());
        
        std::cout << std::left << std::setfill(' ') << std::setw(20) << opcodeToString(opcode) << " ";

        std::cout.flags(f);
   }

   std::shared_ptr<Global> global;

   std::array<std::string, 6> inverseCompareOps_ =
   {
    "<", ">", "==", ">=", "<=", "!="
   };
};

#endif