const assert = require('assert');
const {test} = require('./test-util');

module.exports = eva => {

  /* --- 순서 --- 
  - Point 라는 class 에 대해 새로운  Environment 생성
    - Point Environment 에 {constructor, lambda} , {calc, lambda} 에 해당하는 함수를 변수로 등록

  - (var p (new Point 10 20))
    - 'p' 라는 이름으로 instance Environment 변수를, Global Environment 에 등록한다.
    - (new Point 10 20)
      - Global Environment 에 등록되어 있는 Point Class 에 대한 Environment 를 가져온다.
      - Point Environment 를 parent 하는, instance 용 Environment 를 만든다.
      - Point Envrionment 에 있는 'constructor' 변수를 찾아서 실행한다.
          - instance Environment 에 record 내용을 추가한다. (_callUserDefinedFunction)
            ex) this => instance Environment
                x    => 10
                y    => 20
          - (set (prop this x) x)
              - this 에 해당하는 environment 를 가져온다. 이때 environment 는 instance Environment
              - this 에 대응되는, instance Environment 에 'x' 라는 변수를 '20'/ 'y' 라는 변수를 '10' 이라는 값으로 정의한다. 

  - ((prop p calc) p)
    - 'p' 라는 이름의 'envrioment' 에서 'calc' 라는 '변수'를 찾는다.
       즉, p 라는 instance 에 등록되어 있는 'calc' 라는 멤버 함수를 찾는다.
    - _callUserDefinedFunction 함수를 실행 => 'calc' 라는 멤버 함수를 실행하는 것
      - 새로운 envrioment 생성
      - 해당 environment 에서 'this'라는 이름의 변수로 'p' 라는 instance 등록
        - (+ (prop this x) (prop this y)) : x 와 y 를 더한다.

  */
  test(eva,
      `
      (begin
        (class Point null
          (begin
            (def constructor (this x y)
              (begin
                (set (prop this x) x)
                (set (prop this y) y))
              )
            (def calc (this)
              (+ (prop this x) (prop this y))
            )
          )
        )
    
        (var p (new Point 10 20))
    
        ((prop p calc) p)
      )
    
      `,
  30);

  // test(eva,
  // `
  //   (class Point3D Point
  //     (begin
// 
  //       (def constructor (this x y z)
  //         (begin
  //           ((prop (super Point3D) constructor) this x y)
  //           (set (prop this z) z)))
// 
  //       (def calc (this)
  //         (+ ((prop (super Point3D) calc) this)
  //            (prop this z)))))
// 
  //   (var p (new Point3D 10 20 30))
// 
  //   ((prop p calc) p)
// 
  // `,
  // 60);

};