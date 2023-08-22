#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;

    EvaValue result = vm.exec(R"(
      (def square (x) (* x x))
      (lambda (x) (* x x))
    )");

    log(result);

    return 0;
}
