#ifndef __EvaVM_h

#define __EvaVM_h

#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "../parser/EvaParser.h"
#include "../compiler/EvaCompiler.h"
#include "./EvaValue.h"
#include "./Global.h"

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
#define READ_BYTE()  *ip++


public :
    EvaVM() : 
    parser(std::make_unique<EvaParser>()),
    global(std::make_shared<Global>()),
    compiler(std::make_unique<EvaCompiler>(global))
    {
        std::cout << "evaVM constructor" << std::endl;
        sp = stack.data();
        setGlobalVariables();
    };

    EvaValue exec(const std::string& program)
    {
        // 1. parse program to ast
        auto ast = parser->parse(program);

        // 2. compile program to Eva bytecode
        // compiler : accepts ast => produce bytecode & associated data structure (ex. constant pool)
        co = compiler->compile(ast);

        // Set instruction pointer to first byte of bytecode (혹은 program counter 라고도 불린다.)
        ip = &co->code[0];

        // Debug disassembly
        compiler->disassembleBytecode();

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
                case OP_COMPARE :
                    {
                    auto op = READ_BYTE();

                    auto op2 = pop();
                    auto op1 = pop();

                    if (IS_NUMBER(op1) && IS_NUMBER(op2))
                    {
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);
                        compare_values<int>(op, v1, v2);
                    }
                    else if (IS_STRING(op1) && IS_STRING(op2))
                    {
                        auto v1 = AS_CPPSTRING(op1);
                        auto v2 = AS_CPPSTRING(op2);
                        compare_values<std::string>(op, v1, v2);
                    }

                    break;   
                    }
                case OP_JMP_IF_FALSE :
                    {
                    // condition 조사 결과가 stack top 에 있을 것
                    auto cond = AS_BOOLEAN(pop());

                    // condition 이 false 라면, else 쪽으로 넘어가야 한다.
                    // 이때, compiler 에서 bytecode 를 만들어줄 때, else 분기 시작점에 대한
                    // address 를 2byte 로 구성했기 때문에 2byte 를 읽는다.
                    auto elseAddressOffset = read_short();

                    if (!cond)
                    {
                        std::cout << "jump to elseAddres : " << elseAddressOffset << std::endl;

                        // else 시작점으로 이동하기 
                        ip = to_address(elseAddressOffset);
                    }
                    break;
                    }
                case OP_JMP :
                    {
                        // ex. (if (> 5 1) 2 3) 이라고 하면, '2' 로 간 다음, 그 이후에 byte code 로 만들어진
                        //     3 부분에 대한 것을 건너뛰는 것이다. 그래서 if 가 true 일때에 대한 경우만 처리하는 것이다.
                        ip = to_address(read_short());
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
    uint16_t read_short()
    {
        // ip[-2] : 상위 8byte, ip[-1] : 하위 8byte 정보 저장
        ip += 2;
        return (uint16_t)((ip[-2] << 8) | ip[-1]);
    }

    EvaValue get_const()
    {
        size_t constantIndex = READ_BYTE();
        return co->constants[constantIndex];
    }

    uint8_t* to_address(size_t offset)
    {
        return &(co->code[offset]);
    }

    template<typename T>
    bool compare_values(size_t op, const T& v1, const T& v2)
    {
        do                                        
        {                               
            bool res;                   
            switch(op)                  
            {                           
                case 0 :                
                {                       
                    res = v1 < v2;        
                    break;              
                }                       
                case 1 :                
                {                       
                    res = v1 > v2;      
                    break;              
                }                       
                case 2 :                
                {                       
                    res = v1 == v2;     
                    break;              
                }                       
                case 3 :                
                {                       
                    res = v1 >= v2;     
                    break;              
                }                       
                case 4 :                
                {                       
                    res = v1 <= v2;     
                    break;              
                }                       
                case 5 :                
                {                       
                    res = v1 == v2;     
                    break;              
                }                       
            }                           
            push(BOOLEAN(res));         
            return res;   
        }while(false);        
    }

    /*
    * BuiltIn variables & functions preinstalled
    */
    void setGlobalVariables()
    {
        global->addConst("x", 10);
        global->addConst("y", 20);

        std::cout << "set globals" << std::endl;
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
   * Global object
    - shared ptr : shared among compiler & vm
   */
  std::shared_ptr<Global> global;

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