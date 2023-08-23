#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;

    EvaValue result = vm.exec(R"(
      (def square (x) (* x x))
      (var square2 (lambda (x) (* x x)))
      ((lambda (x) (* x x)) 2)
      (square 3)
      (square2 3)
    )");

    log(result);

    return 0;
}
