#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;
    
    /*
    < 분석 >
    --- Compiler ---
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
      이후 전역 co->constants 에 'squaure' function 에 대한 function Object 를 추가한다.

    - emit(OP_CONST); emit(crrentCo->constants.size() - 1); 추가

    - 전역 co 이므로 global-define(square) 라 하여, global 에 square 함수를 정의한다.

    2. (square 2)
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
