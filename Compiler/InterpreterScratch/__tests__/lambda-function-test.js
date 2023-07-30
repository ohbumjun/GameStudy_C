const assert = require('assert')
const {test} = require('./test-util')

module.exports = eva =>
{
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
};

// --- 순서 --- 
// - onClick body 등록

// - (onClick (lambda (data) (* data 10))) 실행

// - GlobalEnvironment 에서 onClick 함수에 대한 function 을 찾는다.

// - lambda (data) (* data 10) 라는 인자 분석 및 lambda function 리턴 + 새로운 Environment 에
//   lambda function 을 record 에 등록
//   이때 onClick 함수의 'callback' 이라는 key 로 해당 lambda function 을 등록한다 

// - lambda function 이 등록된 새로운 Environment 내에서 onClick 함수 내부 (callback (+ x y)) 실행

// - callback 이라는 exp 를 만나면, record 로 부터 lambda function 을 찾아서 함수 실행
 