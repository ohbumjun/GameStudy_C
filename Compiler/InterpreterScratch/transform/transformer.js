/*
AST Transformer
- switch -> if 문
- for    -> while 문

즉, sysntatic sugar 에 해당하는 문법? 으로 바꿔주는 class
*/

class Transformer
{
    //     (def square (x) (* x x))
    //     == Syntatic sugar for : (var square (lambda (x) (* x x)))
    //     즉, 함수를 선언한다는 것은, 그저 함수 이름에 대응되는 lambda 함수를 만든다는 것과 동일하다
    //     즉, 사실상 아래의 기능은 런타임
    transformDefToLambda(defExp)
    {
        const [_tag, name, params, body] = defExp;
        const varExp = ['var', name, ['lambda', params, body]];
        return varExp;
    }

    transformSwithToIf(switchExp)
    {
        const [_tag, ...cases] = switchExp;

        const ifExp = ['if', null, null, null];

        let current = ifExp;

        for (let i = 0; i < cases.length - 1; ++i)
        {
            const [currentCond, currentBlock] = cases[i];
            current[1] = currentCond;
            current[2] = currentBlock;

            // 마지막 else 문 체크
            const next = cases[i + 1];
            const [nextCond, nextBlock] = next;

            current[3] = nextCond == 'else' ? nextBlock : ['if'];

            // 계속해서 ifExp 에 nested if 문 형태로 채워넣는 것이다.
            // ex) [if, [ '=', 'x', 10 ], 100, [ 'if', [ '>', 'x', 10 ], 200, 300 ]]
            current = current[3];

        }

        ifExp.forEach(exp=>{console.log(exp)})

        return ifExp;
    }
}

module.exports = Transformer;
