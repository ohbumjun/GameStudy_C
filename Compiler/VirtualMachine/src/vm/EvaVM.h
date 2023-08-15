#ifndef __EvaVM_h

#define __EvaVM_h

#include "../bytecode/OpCode.h"
#include "../utils/Logger.h"
#include "../parser/EvaParser.h"
#include "../compiler/EvaCompiler.h"
#include "./EvaValue.h"
#include "./Global.h"

#include <vector>
#include <stack>
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
Stack Frame for functino calls
- EvaVM 이 가지고 있는 evaValue stack 과 별개로
- 여러개의 stack frame 으로 구성되어 있는 Call Stack 을 따로 구성할 것이다.
*/
struct Frame
{
    // return addr of caller 
    // 즉, 특정 함수를 호출하고 나서, 다시 돌아올 address 를 keep 해둔다.
    uint8_t* ra;

    // base pointer of prev caller
    // 이전 함수의 local variable 들에 접근할 수 있게 된다.
    EvaValue* bp;

    // 이전 Function Object 
    FunctionObject* fn;
};

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
        setGlobalVariables();

        std::cout << "EvaVM Constructor" << std::endl;
    };

    EvaValue exec(const std::string& program)
    {
        // 1. parse program to ast
        auto ast = parser->parse("(begin " + program + ")");

        // 2. compile program to Eva bytecode
        // compiler : accepts ast => produce bytecode & associated data structure (ex. constant pool)
        std::cout << "--- Compile ---" << std::endl;

        compiler->compile(ast);

        fn = compiler->getMainFunction();

        // Set instruction pointer to first byte of bytecode (혹은 program counter 라고도 불린다.)
        ip = &fn->co->code[0];

        sp = stack.data();

        bp = sp;

        // Debug disassembly
        compiler->disassembleBytecode();

        std::cout << "--- VM eval ---" << std::endl;

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
            dumpStack();

            auto opcode = READ_BYTE();
            
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
                
                case OP_GET_GLOBAL : 
                {
                    auto globalIndex = READ_BYTE();

                    // push global value on to the stack
                    push(global->get(globalIndex).value);

                    break;
                }

                case OP_SET_GLOBAL :
                {
                    auto globalIndex = READ_BYTE();
                    
                    // global->variables[globalIndex] 에 해당하는
                    // 변수의 값을 조회한다.
                    // 컴파일러 : value -> OP_SET_GLOBAL -> index
                    //           여기서 value 에 해당하는 값을 가져온다.
                    // ex) var x 3 : 3 stack 에 push -> x 변수 정의 -> x 변수의 idx stack push 
                    auto value = peek(0);
                    
                    global->set(globalIndex, value);

                    break;
                }

                case OP_GET_LOCAL : 
                {
                    auto localIndex = READ_BYTE();

                    if (localIndex < 0 || localIndex >= stack.size())
                    {
                        DIE << "OP_GET_LOCAL : invalid variable index " << (int)localIndex;
                    }

                    // bp : base pointer (fp - frame pointer)
                    push(bp[localIndex]);

                    break;
                }

                case OP_SET_LOCAL :
                {
                    auto localIndex = READ_BYTE();

                    if (localIndex < 0 || localIndex >= stack.size())
                    {
                        DIE << "OP_GET_LOCAL : invalid variable index " << (int)localIndex;
                    }
                    auto value = peek(0);

                    bp[localIndex] = value;

                    break;
                }

                case OP_SCOPE_EXIT :
                {
                    /*
                    Clean up variables
                    - result of block : on top of stack
                    - variables sit right below the result of a block
                      so we move the result below, which will be the new top after popping the variables
                    */

                    // how many vars to pop
                    auto count = READ_BYTE();

                    // 과정 : pop result of blocks -> pop local vars -> pop local vars
                    // result of block 을, local variable 첫번째 위치로 이동시키기
                    *(sp - 1 - count) = peek(0);

                    // pop local vars
                    popN(count);

                    break;
                }

                case OP_POP :
                {
                    pop();
                    break;
                }

                case OP_CALL :
                {
                    // how many arg
                    auto argCount = READ_BYTE();

                    // stack : fn, param1, param2, param3... 이렇게 param 목록 바로 아래에 존재
                    auto fnValue = peek(argCount);

                    // 1. Native fn
                    if (IS_NATIVE(fnValue))
                    {
                        AS_NATIVE(fnValue)->func();   

                        // after call, result is on top of stack
                        // below result, fn args are located
                        // below param , fn is located
                        // 1. pop result
                        // 2. pop param + fn
                        // 3. push result
                        auto result = pop();

                        popN(argCount + 1);

                        push(result);

                        break;
                    }

                    // 2. User Defined
                    auto callee = AS_FUNCTION(fnValue);

                    // 자. user defined fn 을 호출하고 나서, 다시 user defined fn 호출 전의 명령어 위치로
                    // 되돌아가야 한다. 
                    // 다시 말해 ip(return addr) , bp, fn 정보를 keep 해두었다가 fn 호출이 끝나면 복구시켜야 한다.
                    // 관련 값들을 저장하기 위해서 evaValue stack 외에 call stack 도 만들 것이다.
                    // 그리고 해당 call stack 은 stack frame 들로 구성되어 있게 된다.
                    // 해당 callStack 에서 Frame 을 다시 pop 하여, user defined fn 이전 call stack 으로 돌아가는
                    // bytecode 는 OP_RETURN 이 된다.
                    callStack.push(Frame{ip, bp, fn});

                    // To access local vars , etc in user defined function
                    fn = callee;

                    // set base (frame) pointer for callee
                    // -1           : function 그 자체
                    // argCount     : function result
                    // 즉, bp 가 stack 상에서 fn 의 위치를 가리키게 한다.
                    bp = sp - argCount - 1;

                    // user defined function 을 호출하는 방법은 main function 을 실행하는 방법과 동일하다.
                    // ip 를 bytecode 의 시작점으로 둔다.
                    // 즉, function 을 호출한다는 것은 결국, 그 function 의 bytecode 로 jump 하는 것과 같다.
                    ip = &callee->co->code[0];


                    break;
                }

                /*Return from function*/
                case OP_RETURN :
                {
                    // Restore prev function before returned user defined fn
                    auto callerFrame = callStack.top();

                    ip = callerFrame.ra;
                    bp = callerFrame.bp;
                    fn = callerFrame.fn;

                    callStack.pop();

                    break;
                }

                default :
                {
                    DIE << "Unknown opcode in VM : " << std::hex << opcode;
                }
            }
        }
   }

   void popN(size_t count)
   {
    if (stack.size() == 0)
    {
        DIE << "popN() : empty stack.\n";
    }
    sp -= count;
   }

   /*
   * peek element from stack
   */
  EvaValue peek(size_t offset = 0)
  {
    if (stack.size() == 0)
    {
        DIE << "peek() : empty stack.\n";
    }

    // sp - 1 : value on top of stack
    return *(sp - 1 - offset);
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
        return fn->co->constants[constantIndex];
    }

    uint8_t* to_address(size_t offset)
    {
        return &(fn->co->code[offset]);
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
        global->addConst("VERSION", 1);
        
        /*Native square function*/
        global->addNativeFunction(
            "native-square",
            [&](){
                auto x = AS_NUMBER(peek(0));    // stack 에서 함수 인자를 peek 한다.
                                                // pop 하지 않는 이유는, fn body 내부에서도 사용해야 하기 때문이다. 
                push(NUMBER(x*x));
            },
            1                                   // fn 이 끝나고, pop 해야할 지역 및 매개 변수 개수
        );
    }

    /*
    * Dump current stack
    */
    void dumpStack()
    {
        std::cout << "--- Stack ---" << std::endl;
        if (sp == &(*(stack.begin())))
        {
            std::cout << "(empty)" << std::endl;
        }
        EvaValue* csp = sp - 1;
        while (csp >= &(*(stack.begin())))
        {
            std::cout << *csp-- << "\n";
        }
        std::cout << "\n";
    }

    /*
    * Global object
        - shared ptr : shared among compiler & vm
    */
    std::shared_ptr<Global> global;
    
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
    * Base Pointer (or Frame Pointer)
    - 각각 stack 내 시작점을 가라키는 pointer 
    */
    EvaValue* bp;

    /*
    Operand stack
    */
    std::array<EvaValue, STACK_LIMIT> stack;

    /*
    Seperate stack for calls. keep return address
    - 참고 : 실제 vm 들은 EvaValue, Frame 에 해당하는 stack 을 동시에 사용할 수도 있다.
    */
    std::stack<Frame> callStack;

    /*
    *  Current executing function
    */
   FunctionObject* fn;
};

#endif