#ifndef EvaCompiler_h
#define EvaCompiler_h

#include "../parser/EvaParser.h"
#include "../vm/EvaValue.h"
#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "../disassembler/EvaDisassembler.h"
#include "../vm/Global.h"

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
    EvaCompiler(std::shared_ptr<Global> global) : 
        global(global),
        disassembler(std::make_unique<EvaDisassembler>(global))
        {};

    /*
    * Main compile API
      - receives AST
      - make bytecode & related data structures
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
   
    void disassembleBytecode()
    {
        disassembler->disassemble(co);
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
                    // 1. Global vars
                    if (global->exist(exp.string) == false)
                    {
                        DIE << "[EvaCompiler] : refernce error " << exp.string;
                    }

                    emit(OP_GET_GLOBAL);
                    emit(global->getGlobalIndex(exp.string));
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

                    /*
                    * Branch instruction
                      ex. if, while
                          if <test> <result> <alternate>
                    */
                   else if (op == "if")
                   {
                        /*
                        - 전체 구조- 
                        <test>
                        JMP_IF_FALSE
                        else-begin-address 
                        <consequent>
                        OP_JMP
                        else-end-address
                        <alternate>

                        현재 시점에서는, 아직 실제 if, else 에 대한 branch 가 존재하는 것이 아니다
                        그저 plain array of byte 만이 존재할 뿐이다.
                        
                        그와 동시에 test / 조건. 에 따라서 if 혹은 else section 으로
                        jump 할 수 있는 방법도 있어야 한다.

                        따라서, compiler 를 통해 모든 bytecode 를 만든 이후에
                        비로소 if, else section 에 대한 read address 정보를
                        채워주는 과정을 거쳐야 한다.

                        이를 위해 address 를 저장해두는 임시 공간을
                        마련해두는 원리이다.
                        */

                        // Emit <test>
                        gen(exp.list[1]); 

                        // test 를 실행한 이후, stack 의 top 에는
                        // test 에 대한 true, false 가 있을 것이다.
                        // 여기에서의 trick 은, condition 이 false 인 경우에 대해 먼저
                        // 다루는 것이다. test 에 대한 결과가 false 라면
                        // alternate 부분으로 이동할 것이다.
                        emit(OP_JMP_IF_FALSE);

                        // Else branch. Init with 0 address. will be patches
                        // NOTE : we use 2-byte addresses
                        // 자. 여기 이 시점에서는, 아직 alternate 부분을 compile 하지 않은 시점이므로
                        // 어떤 부분으로 jump 할지를 모른다.
                        // 그렇다면, alternate 부분이 모드 generate 되고 나서,
                        // jump 할 address 위치를 여기에 세팅해둔다.
                        // 그런데 , 미래에 세팅될 것이므로 2byte 만큼의
                        // address 공간을 미리 할당해두고, 나중에 해당
                        // address 값을 채워줄 것이다.
                        emit(0);
                        emit(0);

                        auto elseStartTempAddrOffset = getOffset() - 2;

                        // Generate Consequent part
                        gen(exp.list[2]);

                        // add jmp to skip alternate part
                        emit(OP_JMP);

                        // 2-byte address
                        emit(0);
                        emit(0);

                        auto elseBranchEndTempAddrOffset = getOffset() - 2;

                        // Patch the else branch address
                        auto elseBranchStartAddrOffset = getOffset();

                        // else 의 시작, 끝 address 채워주기
                        patchJumpAddress(elseStartTempAddrOffset, elseBranchStartAddrOffset);
                   
                        // Emit <alternate> if we have it
                        // - generate code for altenate branch
                        // - ex. if(){} 만 하고 끝날 수도 있기 때문
                        if (exp.list.size() == 4)
                        {
                            gen(exp.list[3]);
                        }

                        // Patch the end
                        auto endBranchEndAddrOffset = getOffset();

                        patchJumpAddress(elseBranchEndTempAddrOffset, endBranchEndAddrOffset);
                   }

                   // Variable Declaration
                   else if (op == "var")
                    {
                        // 1. Global vars
                        // ex. (var x (+ y 10))
                        global->define(exp.list[1].string);

                        // Initialize
                        gen(exp.list[2]);

                        emit(OP_SET_GLOBAL);
                        emit(global->getGlobalIndex(exp.list[1].string));

                        

                        // 2. Local vars
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
    * Write byte at offset
    */
    void writeByteAtOffset(size_t offset, uint8_t value)
    {
        co->code[offset] = value;
    }

    /*
    * Patch Jump Address
    */
    void patchJumpAddress(size_t offset, uint16_t value)
    {
        // 2byte 씩 나눠서 쓴다.(상위 -> 하위)
        uint8_t upper_byte = (value >> 8) & 0xff;
        uint8_t down_byte  = (value & 0xff);
        writeByteAtOffset(offset,     upper_byte);
        writeByteAtOffset(offset + 1, down_byte);
    }

    /*
    * Returns current bytecode offset
    */
   size_t getOffset() {return co->code.size();}

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
    * Global object
        - shared ptr : shared among compiler & vm
    */
    std::shared_ptr<Global> global;

    /*
    * Compile code object
    */
    CodeObject* co;
    
    /*
    * Disassembler
    */
    std::unique_ptr<EvaDisassembler> disassembler;

    /*
    * Compare ops -> encode to number
    */
   std::map<std::string, uint8_t> compareOps_ = {
    {"<", 0}, {">", 1}, {"==", 2}, {"<=", 4}, {">=", 5}
   };
};

#endif