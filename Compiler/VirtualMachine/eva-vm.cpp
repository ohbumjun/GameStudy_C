#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;
    
    /*
    < 분석 >
    --- Compiler ---
    0. default 'begin' 으로 인해 Compiler 상에서 scopeEnter() 실행

    1. (def sqaure (x) (* x x))

    - sqaure 라는 이름의 새로운 codeObject 할당

    - crrentCo 를 새로운 codeObject 로 대체

    - 전역 co 의 constants 목록에 square codeObject 추가

    - square codeObject 의 local 목록에 sqaure 이라는 함수 추가, 인자 목록도 local 로 추가

    - (* x x) 라는 function body 에 대해서도 gen 
        - 'x' 라는 symbol 에 대해서 처리할 때는, square coObject 에서 방금 local 에 추가한 x 를 찾아서
          emit(OP_GET_LOCAL) emit(localIndex) 를 진행해준다.
        - 이렇게 gen(x), gen(x), emit(OP_MUL) 을 실행한다.

    - 이후, 함수를 빠져나오기 전에, 해당 함수 coObject 에서 evaValue stack 에 쌓아준 대상들을 pop 하기 위해서 
      emit(OP_SCOPE_EXIT) emit(arity + 1) 를 진행한다. 

    - 'square' 라는 함수 호출 이전의 callstack 으로 복귀하기 위한 byteCode 인 OP_RETURN 을 추가한다.
      emit(OP_RETURN)

    - 'sqaure' 이라는 coObject 를 wrap 하는 function Object 를 할당한다.

    - codeObject 를 'square' 에서 다시 '전역' code Object 로 돌아온다.
      이후 전역 co->constants 에 'squaure' function 에 대한 function Object 를 추가한다.

    - emit(OP_CONST); emit(crrentCo->constants.size() - 1); 추가

    - 전역 co 이므로 global-define(square) 라 하여, global 에 square 함수를 정의한다.
      emit(OP_SET_GLOBAL), emit('square' 의 globalIndex)

    2. default 'begin' 내부에서, OP_POP 을 넣어준다. 이는 '전역' codeObject 의 byteCode 로 들어간다.

    3. (square 2)
    - 전역 co 에서 square 에 해당하는 녀석을 찾는다. emit(OP_GET_GLOBAL) , emit(global index) 를 넣어준다.

    - 2 라는 인자에 해당하는 evaValue 를 만들어서 '전역' codeObject->constants에 push 한다 
      이후 emit(OP_CONST) emit(constant pool 상의 idx) 를 넣어준다.

    - 마자믹에 emit(OP_CALL) 과 emit(param 개수) 진행

    4. default 'begin' 을 scopeExit 를 통해 빠져나온다.
        - '전역' scope -> coObject.locals 에서 '전역' scopeLevel 과 같은 대상을 pop 한다
            - 하지만, 여기서는 아무값도 pop 되지 않을 것이다. '전역' coObject 에 대해 addLocal 을 한 적이 없기 때문이다.
            - 'sqauare' 라는 fn 은 'square' 의 coObject 에 addLocal 로 추가했고, '전역' coObject 에는 아무값도 없다. 
        - EvaCompiler::scopeExit 함수 내에서 varsCount도 0 이고, arity 도 0 이므로, 더이상의 emit 을 하지 않고 빠져나온다.
    --- VM ---
    1. '전역 codeObject 처리'
      1) CONST 자. ip 는 맨 처음에 main fn obj->co->code[0] 을 가리킨다. 즉, '전역' codeObject 의 bytecode 부터 진행한다.
      - 전역 fn->co->constants 에서 'square' function object 를 찾아서 stack 에 push 한다

      2) SET_GLOBAL
      - global->define('square') 로 정의된 GlobalVar 에, stack 에 push 되었던 'square' function object evaValue 를 세팅

      3) POP
      - stack 에서 'square' function object evaValue 를 pop 한다.
        참고 : stack 의 top 에는 항상 result 가 존재해야 한다. 'square' function object evaValue 에 대한 작업을
              마무리했으므로 stack 에서 제거해주는 것이다.

      4) GET_GLOBAL
      - global 의 GlobalVar 들 중에서 'square' 에 해당하는 GlobalVar 를 찾아서 stack 에 push 한다

      5) CONST
      - 이제 'square' 에 해당하는 'parameter' 를 '전역' codeObjet 의 constant pool 에서 찾아서 stack 에 push 한다.
      - 여기까지 하면 (btm) square fn evaValue, param evaValue (top) 와 같은 형태로 stack 이 쌓여있게 된다.
      
      5) CALL
      - 인자 개수를 '전역' codeObject 로 부터 읽는다 (ip++)
      - stack 에서 square fn evaValue 를 peek 한다.
      - callStack 에 , '전역' ip,bp,fn 정보를 추가하여, 차후 square fn 실행 이후, 어디로 돌아갈지를 저장해둔다.
      - 이 시점 부터, ip 는 square codeObject 를 가리키고, bp 는 stack 상에서 squaure fn evaValue 가 있는 곳을 가리킨다 (stack의 시작점이 아니라)

      {
        (def sqaure (x) (* x x))
        1) GET_LOCAL 2번
        - 자. localIndex = READ_BYTE() 를 통해 'x' 라는 변수가 squaure codeObject->locals 에서 몇번째 idx 에 있는지를 읽는다
          READ_BYTE() 는 ip++ 를 통해 읽는데 기억할 점은 이제 ip 는 '전역' codeObject 가 아니라 square codeObject 라는 것이다.
        - bp[localIndex] 에 해당하는 evaValue 를 evaValue stack 에 push 한다.
          마찬가지로 bp 는 이제 evaValue stack 0번째가 아니라, evaValue stack 에서 squaure fn evaValue의 시작점을 가리키고 있다
          그리고 stack 에는 (bottom) ... square fn evaValue, param evaValue == 'x' 라는 param 이 '2' 라는 값을 지닌다는 것을 의미하는 evaValue ... (top) 형태로 있게 되고
          bp[localIndex] 는 결국 'x' 라는 param 에 대한 evaValue 를 리턴한다.

          즉, 'x' param evaValue 를 2번 stack 에 push 한다.

          여기까지 실행하면, (bottom) ... square fn evaValue, param evaValue, param evaValue, param evaValue ... (top) 이렇게 총 4개의 값을 stack 이 지니게 된다.
        
        2) MUL
        - 'x' param evaValue 에는 '2'라는 값이 들어있다. 2번 pop 하고, 곱한 결과값 4를 evaValue 로 만들어서 다시 stack 에 push 한다.

        3) SCOPE_EXIT 
        - 현재 stack 에는 (bottom) .. square evaValue , param evaValue == 'x' 라는 param 이 '2' 라는 값을 지닌다는 것을 의미하는 evaValue , 4 (mul 연산 결과) .. (top) 이 들어있다
          4 라는 mul 연산 결과 제외 하고, 그 아래 2개 값을 pop 한다.
          현재 square 라는 fn 내에서 만들어진 evaValue 들을 stack 에서 제거하는 것이다. 

        4) RETURN
        - callStack 에 있던, '전역' codeObject 관련 callStack 으로 다시 복구 시킨다.

        5) OP_HALT
        - stack 에 있는 '4' 라는 결과값을 pop 하여 리턴한다.
      }
    */
    EvaValue result = vm.exec(R"(
      (def square (x) (* x x))
      (square 2) // 4
      // (def sum (a b)
      //   (begin
      //     (var x 10)
      //     (+ x (+ a b))
      //   )
      // )
    )");

    log(result);

    return 0;
}
