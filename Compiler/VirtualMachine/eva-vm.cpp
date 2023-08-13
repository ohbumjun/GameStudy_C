#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;
    
    /*
    < 분석 >
    1) (var x 3)
    - op_const 라는 bytecode 추가
    - '3' 이라는 숫자를 EvaValue 형태로 byteCode
    - global 에 'x' 라는 변수 정의
    - op_set_global 이라는 bytecode 추가
    - 'x' 의 global index 값 bytecode 에 추가

    2) (square x)
    - 'square' 라는 함수를 찾는다
    - op_get_global byte code 추가
    - 'square' 라는 함수의 idx bytecode 에 추가

    - 'x' 라는 global 변수를 찾는다.
    - op_get_global byte code 추가
    - 'x' 라는 변수의 global idx 를 bytecode 에 추가 

    - op_call bytecode 추가
    - call 이후, pop 할 param 개수 byte code 추가 

    */
    EvaValue result = vm.exec(R"(
      (var x 3)
      (square x) // 4
    )");

    log(result);

    return 0;
}
