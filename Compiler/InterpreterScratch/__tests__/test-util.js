/*
실제 s-expression format 을 통해서 test 를 위한 더 편한 format 을 사용할 것이다.
참고로 Javascript 의 array 는 s-expression 과 1:1 대응관계를 이룬다.

testUtil.test(`
        (begin
            (var x 10)
            (var y 20)
            (+ (* x 10) y))
    `, 120);

    즉, test method 는 s-expression 형태의 string 을 받아서
    실행하는 형태가 될 것이다.

    그리고 s-expression 을 parse 해서 AST format 으로 변경해주는
    작은 parser 을 만들고자 한다.

    "Syntax" tool 이라는 것을 사용할 것이다.
    이는 Parser Generator 로 BNF grammer 로 부터
    automatic parser 을 만들어내는 역할을 한다.


즉,
1) testUtil.test 함수에 s-expression 형태의 string 을 넘긴다.
2) "Syntax tool" 을 활용한다. 
   BNF 문법 -> Parser 를 만들어낸다. == evaParser.js
3) evaParser.js 가 s-expression 을 parsing 해서 기존에 우리가 사용하던 
 ['begin',
            ['var', 'x', 10],
            ['var', 'y', 20],
            ['+', ['*', 'x', 'y'], 30],
        ]
  와 같은 형태를 만들어줄 것이다.
4) 이를 그대로 eva.eval() 함수에 인자로 넘겨준다.
*/

const assert = require('assert');
const evaParser = require('../parser/evaParser')

function test(eva, code, expected)
{
    // 우선 string 을 실제 parse 한다.
    const exp = evaParser.parse(`(begin ${code})`);
    assert.strictEqual(eva.evalGlobal(exp), expected);
}

module.exports = {
    test,
}