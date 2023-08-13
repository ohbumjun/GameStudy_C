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

/*
1) https://www.youtube.com/watch?v=IR_L1xf4PrU

- C 코드를 cpu 가 이해하거나 실행할 수 있는 기계어로 바꿔줘야 한다.
  .C / .CPP => x86, ARM ..

2) clang 이란 ?


https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=yong030389&logNo=221698524595
https://growingdev.blog/entry/%EA%B0%9C%EB%B0%9C-%ED%99%98%EA%B2%BD-LLVM%EA%B3%BC-Clang%EC%97%90-%EB%8C%80%ED%95%B4%EC%84%9C
https://doitnow-man.tistory.com/100
https://www.youtube.com/watch?v=FnGCDLhaxKU&list=PLGvfHSgImk4YrUBPDUVczCg6LF8JEGJXN

FrontEnd ... 어떻게 코드를 이해할 것인가
이제 핵심은, 어떻게 최적화 할 것인가
C++ 언어 사용하면 무조건 빠른게 아니다. 단, c++ 는 우리에게 강력한 선택권 및 옵션을 제공하는 것이다.
최적화 하기 위해서..!
*/