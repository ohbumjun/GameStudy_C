const assert = require('assert')
const {test} = require('./test-util')

module.exports = eva =>
{
    /* --- 순서 --- 
    // - onClick body 등록

    // - (onClick (lambda (data) (* data 10))) 실행

    // - GlobalEnvironment 에서 onClick 함수에 대한 function 을 찾는다.

    // - lambda (data) (* data 10) 라는 인자 분석 및 lambda function 리턴 + 새로운 Environment 에
    //   lambda function 을 record 에 등록
    //   이때 onClick 함수의 'callback' 이라는 key 로 해당 lambda function 을 등록한다 

    // - lambda function 이 등록된 새로운 Environment 내에서 onClick 함수 내부 (callback (+ x y)) 실행

    // - callback 이라는 exp 를 만나면, record 로 부터 lambda function 을 찾아서 함수 실행
    */
    test(eva, `
        (begin
            (def onClick (callback)
                (begin
                    (var x 10)
                    (var y 20)    
                    (callback (+ x y))
                )
            )
            (onClick (lambda (data) (* data 10)))
        )
    `, 
    300);

    /*
    IILE : Immediately-invoked lambda expression
    */
    /* --- 순서 --- 
    - lamda 에 대한 함수 생성
    - lamda 의 param 'x' ~~ 인자 '2' 에 대한 내용을 record 에 등록 및 새로운 Environment 생성
      (즉, lambda 를 생성하는 순간 사실상 새로운 block 이 만들어진다는 것이다.)
    - 이후 lambda 의 body 부분을 실행하면서 'x' 변수를 '2' 로 lookup 헤서 결과물을 리턴한다.
    */
    test(eva, `
        ((lambda (x) (* x x)) 2)
    `, 
    4);

    // Save lambda to variable
     /* --- 순서 --- 
    - squaure => lambda 에 해당하는 함수로 Environment 에 등록
    - (square 2) 실행
        square 에 해당하는 함수는 param : x / body : (* x x)
        param 'x' : 인자 2 등록
        이후 body 를 실행하면서 (* 2 2) 실행
    */
    test(eva, `
        (begin
            (var square (lambda (x) (* x x)))
            (square 2)
            )
    `, 
    4);
};

