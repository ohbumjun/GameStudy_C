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
        for (auto i = 0; i < crrentCo->constants.size(); ++i) \
        {                                               \
            if (!tester(crrentCo->constants[i]))              \
            {                                           \
                continue;                               \
            }                                           \
            if (converter(crrentCo->constants[i]) == value)   \
            {                                           \
                return i;                               \
            }                                           \
        }                                               \
        crrentCo->constants.push_back(allocator(value));      \
    }while(false);      



public :
    EvaCompiler(std::shared_ptr<Global> global) : 
        global(global),
        disassembler(std::make_unique<EvaDisassembler>(global))
        {
            std::cout << "Compile constructor" << std::endl;
        };

    /*
    * Main compile API
      - receives AST
      - make bytecode & related data structures
    */
    void compile(const Exp& exp)
    {
            crrentCo = AS_CODE(createCodeObjectValue("main"));

            main = AS_FUNCTION(ALLOC_FUNCTION(crrentCo));

            // Generate recursively from top-level (functions ?)
            gen(exp); 

            // explicit vm-stop marker
            emit(OP_HALT);
            
            std::cout << "compile end" << std::endl;
    }

    FunctionObject* getMainFunction() {return main;}
   
    void disassembleBytecode()
    {
        for (auto& co : codeObjects)
        {
            disassembler->disassemble(co);
        }
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
                // std::cout << "symbol in gen : " << exp.string << std::endl;

                // ex. true
                if (exp.string == "true" || exp.string == "false")
                {
                    emit(OP_CONST);
                    emit(booleanConstIdx(exp.string == "true" ? true : false));
                }
                else 
                {
                    // variables
                    auto varName = exp.string;

                    // 1. Local vars
                    auto localIndex = crrentCo->getLocalIndex(varName);

                    if (localIndex != -1)
                    {
                        emit(OP_GET_LOCAL);
                        emit(localIndex);
                    }
                    else 
                    {
                        // 2. Global vars
                        if (global->exist(varName) == false)
                        {
                            DIE << "[EvaCompiler] : reference error " << varName;
                        }

                        emit(OP_GET_GLOBAL);
                        emit(global->getGlobalIndex(varName));
                    }
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
                        // std::cout << "* op" << std::endl;

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
                        // ex. (var x (+ y 10))
                        auto varName = exp.list[1].string;

                        // special treatment of (var foo (lambda ...))
                        // to capture function name from variables
                        if (isLambda(exp.list[2]))
                        {
                            compileFunction(
                            /*exp*/     exp.list[2],
                            /*name*/    varName,
                            /*params*/  exp.list[2].list[1],
                            /*body*/    exp.list[2].list[2]
                        );
                        }
                        else 
                        {
                            // Initialize
                            gen(exp.list[2]);
                        }

                        // 1. Global vars
                        if (isGlobalScope())
                        {
                            global->define(varName);

                            emit(OP_SET_GLOBAL);
                            emit(global->getGlobalIndex(exp.list[1].string));
                        }
                        
                        // 2. Local vars
                        else 
                        {
                            crrentCo->addLocal(varName);
                            emit(OP_SET_LOCAL);
                            emit(crrentCo->getLocalIndex(varName));
                        }
                    }

                    // function declaration
                    // ex) (def <name> <param> <body>)
                    // Sugar for : (var <name> (lambda <params> <body>))
                    // 즉, def 를 통해 정의하는 함수는 사실상, 변수에 lambda 함수를 정의하는 것과 동일하다.
                    else if (op == "def")
                    {
                        auto fnName = exp.list[1].string;
                        /*auto params = exp.list[2].list;   */
                        /*auto body   = exp.list[3];        */
                        /*auto arity  = params.size();      */

                        compileFunction(
                            /*exp*/     exp,
                            /*name*/    fnName,
                            /*params*/  exp.list[2],
                            /*body*/    exp.list[3]
                        );
                        
                        // define fn in currentCO
                        if (isGlobalScope())
                        {
                            global->define(fnName);
                            emit(OP_SET_GLOBAL);
                            emit(global->getGlobalIndex(fnName));
                        }
                        else 
                        {
                            crrentCo->addLocal(fnName);
                            emit(OP_SET_LOCAL);
                            emit(crrentCo->getLocalIndex(fnName));
                        }
                    }
                    // Lambda Expression
                    else if (op == "lambda")
                    {
                        compileFunction(
                            /*exp*/     exp,
                            /*name*/    "lambda",
                            /*params*/  exp.list[1],
                            /*body*/    exp.list[2]
                        );
                    }

                    // Variable Set
                    else if (op == "set")
                    {
                        auto varName = exp.list[1].string;

                        gen(exp.list[2]);

                        // 2. Local vars
                        auto localIndex  = crrentCo->getLocalIndex(varName);
                        
                        if (localIndex != -1)
                        {
                            emit(OP_SET_LOCAL);
                            emit(localIndex);
                        }
                        else
                        {
                            // 2. Global vars
                            // ex. (var x (+ y 10))
                            auto globalIndex = global->getGlobalIndex(varName);

                            if (globalIndex == -1)
                            {
                                DIE << "Reference error : " << varName << " is not defined";
                            }
                            emit(OP_SET_GLOBAL);
                            emit(globalIndex);
                        }
                    }

                    // while loop eX) while <test> <body>
                    else if (op == "while")
                    {
                            // while loop 의 시작 주소를 계속 갖고 있어서, while 조건이 true 라면
                            // loop 마다 해당 주소로 다시 돌아와야 한다.
                            auto loopStartAddr = getOffset();

                            // Emit <test>
                            gen(exp.list[1]);

                            // loop end. Init with 0 address, will be patched
                            // 만약 condition 리 false 라면, while body 문을 빠져나가야 한다.
                            emit(OP_JMP_IF_FALSE);

                            // we don't know end addr of body
                            // 2byte address
                            emit(0);
                            emit(0);

                            auto loopEndJmpAddr = getOffset() - 2;

                            // generate actual body
                            gen(exp.list[2]);

                            // jump to begin of while loop
                            emit(OP_JMP);

                            // while loop 시작 주소를 세팅한다.
                            emit(0);
                            emit(0);

                            // while loop 주소 세팅
                            patchJumpAddress(getOffset() - 2, loopStartAddr);

                            // we now know the end addr of body
                            auto loopEndAddr = getOffset() + 1;

                            patchJumpAddress(loopEndJmpAddr, loopEndAddr);
                    }
                    // Blocks
                    else if (op == "begin")
                    {
                        scopeEnter();
                        for (auto i = 1; i < exp.list.size(); ++i)
                        {
                            // value of the last expression is kept on the stack as final result
                            bool isLast = i == exp.list.size() - 1;

                            // Local variable or function (should not be popped)
                            // - popped from stack when scope ends
                            auto isLocalDeclaration = isDeclaration(exp.list[i]) && !isGlobalScope();

                            // generate expression code
                            gen(exp.list[i]);

                            if (!isLast && !isLocalDeclaration)
                            {
                                // 계속해서 OP_POP 을 넣어주는 이유 ?
                                // - ex. (set x (+ x 10)) : 각각의 명령어를 실행하고 나서, 그 다음 명령어를 stack 에서 찾아서 실행할 수 있게 하기 위함이다.
                                emit(OP_POP);
                            }
                        }
                        scopeExit();
                    }
                    // Function Calls
                    // ex. (square 2)
                    else
                    {
                        function_call(exp);
                    }
                }
                // Lambda Function Calls
                // ex. ((lambda (x) (* x x)) 2)
                else 
                {
                    // ExpType::LIST 이지만, tag.type 이 symbol 은 아닌 경우
                    std::cout << "lambda call : " << std::endl; 
                    function_call(exp);
                }

                break;
            }
        }
    }

private :
    bool isBlock(const Exp& exp){return isTaggedList(exp, "begin");}
    bool isDeclaration(const Exp& exp) {return isVarDeclaration(exp);}
    // (var <name> <value>)
    bool isVarDeclaration(const Exp& exp) {return isTaggedList(exp, "var");}
    bool isTaggedList(const Exp& exp, const std::string& tag)
    {
        return exp.type == ExpType::LIST && exp.list[0].type == ExpType::SYMBOL &&
                exp.list[0].string == tag;
    }
    // Num of local vars in this scope
    size_t getVarsCountAndPopLocalsOnScopeExit()
    {
        auto varsCount = 0;

        if (crrentCo->locals.size() > 0)
        {
            while (crrentCo->locals.back().scopeLevel == crrentCo->scopeLevel)
            {
                crrentCo->locals.pop_back();
                varsCount++;
            }
        }

        std::cout << "varCount  in getVarsCountAndPopLocalsOnScopeExit : " << varsCount << std::endl;

        return varsCount;
    }

    EvaValue createCodeObjectValue(const std::string& name, size_t arity = 0)
    {
        auto coValue = ALLOC_CODE(name, arity);
        auto co = AS_CODE(coValue);
        codeObjects.push_back(co);
        return coValue;
    }

    void scopeEnter() {crrentCo->scopeLevel++;}
    void scopeExit()  
    {
        // pop vars from stack if they were declared
        // within this specific scope
        // - 즉, scope 를 나가는 순간 해당 scope 에서 선언된 모든 지역변수 및 function 들도 pop 할 것이다.
        auto varsCount = getVarsCountAndPopLocalsOnScopeExit();

        // varCount > 0         : Block 끝나면서 내부 지역변수 목록 clear
        // crrentCo->arity > 0  : Defined function block 을 끝나고 나올 때, 지역변수 + 매개 변수 개수 + 함수 자체
        if (varsCount > 0 || crrentCo->arity > 0)
        {
            emit(OP_SCOPE_EXIT);

            if (isDefinedFunctionBody())
            {
                // 매개 변수 개수 
                // Defined 함수 자체
                varsCount += crrentCo->arity + 1;
            }
            emit(varsCount);
        }

        crrentCo->scopeLevel--;
    }

    /*
    * Emit data to bytecode
    */
    void emit(uint8_t code)
    {
        crrentCo->code.push_back(code);
    }

    /*
    * Write byte at offset
    */
    void writeByteAtOffset(size_t offset, uint8_t value)
    {
        crrentCo->code[offset] = value;
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
   size_t getOffset() {return crrentCo->code.size();}

    /*
    * Allocate a numeric constant
      - return Idx 
    */
    size_t numericConstIdx(double value)
    {
        ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);

        return crrentCo->constants.size() - 1;
    }

    size_t stringConstIdx(const std::string& value)
    {
        ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
        return crrentCo->constants.size() - 1;
    }

    size_t booleanConstIdx(bool value)
    {
        ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
        return crrentCo->constants.size() - 1;
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

    void function_call(const Exp& exp)
    {
        do {                                            
            gen(exp.list[0]);                           
            for(auto i = 1; i < exp.list.size(); i++)   
            {                                           
                gen(exp.list[i]);                      
            }                                          
            emit(OP_CALL);                             
            emit(exp.list.size() - 1);                  
        } while(false);    
    }
    void compileFunction(const Exp& exp, const std::string& fnName, const Exp& params, const Exp& body)
    {
        auto arity  = params.list.size();
        
        // store previous code object
        auto prevCo = crrentCo;

        // Create New Code Object for function
        /*
        (
            (def sqaure (x) (* x x)) 
        )
        이 경우, (* x x) 부분은, scope level 2 가 아니라 1 or 0 이 된다.
        아예 새로운 CodeObject 를 정의한 것이기 때문이다.
        */
        auto coValue = createCodeObjectValue(fnName, arity);
        crrentCo = AS_CODE(coValue);

        // store new co as constant (for closure)
        prevCo->constants.push_back(coValue);

        // fn registered as local
        crrentCo->addLocal(fnName);

        // params added as variables
        for (auto i = 0; i < arity; ++i)
        {
            auto argName = params.list[i].string;
            crrentCo->addLocal(argName);
        }

        // compile body -> all generated bytecode will be added to newly created codeObject
        gen(body);

        // fn body 가 block 이라면, scope_exit bytecode 로 인해 해당 함수 내 모든
        // 지역 변수 및 매개 변수등을 pop 하게 될 것이다.
        // 하지만, 만약 block 이 아니라면 (즉, begin 이라는 keyword 로 시작하지 않는다면), 별도의 처리를 해줘야 한다.
        // ex) def sqaure (x) (* x x)
        if (!isBlock(body))
        {
            emit(OP_SCOPE_EXIT);

            // + 1. for the newly created function itself
            emit(arity + 1);
        }

        // 1. restore previous instruction ponter address
        // 2. restore previous base pointer for variables
        emit(OP_RETURN);

        // Create the function
        // - 즉, 이 시점에서 알 수 있는 것은 runtime function creation 은 존재하지 않고
        //   모든 함수는 컴파일 타임 때 만들어진다.
        auto fn = ALLOC_FUNCTION(crrentCo);

        // restore the code object (이제부터의 emit 은 user defined code object 가 아니라
        // 기존 codeObject 로 들어가게 된다.)
        crrentCo = prevCo;
        
        // add fn object as constants
        crrentCo->constants.push_back(fn);

        // And emit code for new constant
        emit(OP_CONST);

        emit(crrentCo->constants.size() - 1);
    }

    bool isGlobalScope() {return crrentCo->name == "main" && crrentCo->scopeLevel == 1;}

    /*
    ex. 
    (
        (def sqaure (x) (* x x)) 에서 (* x x) 부분이 isFunctionBody == true 가 된다.
    )
    */
    bool isDefinedFunctionBody() {return crrentCo->name != "main" && crrentCo->scopeLevel == 1;}

    /*
    * (labmda..)
    */
   bool isLambda(const Exp& exp) {return isTaggedList(exp, "lambda");}
    
    /*
    * Main Entry Point (function)
        - 컴파일러로 하여금, main function 에 대한 reference 를 들고 있게 한다.
    */
   FunctionObject* main;

    /*
    * Global object
        - shared ptr : shared among compiler & vm
    */
    std::shared_ptr<Global> global;

    /*
    * Compile code object
    */
    CodeObject* crrentCo;

    /*
    * All Code Objects
    */
    std::vector<CodeObject*> codeObjects;
    
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