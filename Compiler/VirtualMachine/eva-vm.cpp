#include "src/vm/EvaVM.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    EvaVM vm;
    
    EvaValue result = vm.exec(R"(
    (var i 0)
    (var count 0)
    (while (< i 10)
      (begin
        (set i (+ i 1))
        (set count (+ count 1))
      )
    )
      count  
    )");

    log(result);

    return 0;
}
