#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;
    
    /*
    < 분석 >
   
    */
    EvaValue result = vm.exec(R"(
      (def sqaure (x) (* x x))
      (square 2) // 4
      (def sum (a b)
        (begin
          (var x 10)
          (+ x (+ a b))
        )
      )
    )");

    log(result);

    return 0;
}
