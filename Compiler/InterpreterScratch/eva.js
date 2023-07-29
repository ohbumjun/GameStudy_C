const assert = require('assert');
const Environment = require('./environment');

/*
* eva interpreter
- ast 를 받아서, 실제 실행을 해주는 녀석
- ast 는 single 형태로 구성될 것이다.
*/
class Eva
{
    /*
    Create Eva instance with global environment
    */
    constructor(global = new Environment())
    {
        this.global = global;
    }

    eval(exp, env = this.global)
    {

        if (isNumber(exp))
        {
            return exp;
        }
        if (isString(exp))
        {
            return exp.slice(1, -1);
        }
        // Math operator
        if (exp[0] === '+')
        {
            return this.eval(exp[1], env) + this.eval(exp[2], env);
        }
        if (exp[0] === '*')
        {
            return this.eval(exp[1], env) * this.eval(exp[2], env);
        }
        if (exp[0] === '-')
        {
            return this.eval(exp[1], env) - this.eval(exp[2], env);
        }
        if (exp[0] === '/')
        {
            return this.eval(exp[1], env) / this.eval(exp[2], env);
        }

        /*
        Variable 은 Environment 정보까지 같이 가지고 있어야 한다.
        ex) x 라는 변수 -> 지역변수 ? global 변수 ?

        Enviroment 구조
        1) Environment Record
        - 변수 이름 ~ 변수 에 대한 table
        2) Parent Environment 에 대한 reference
        - 자식 Environment 는 Parent 에 있는 variable 에 접근 가능
        - 그 반대는 불가능하다.
        */
        // Variables
        if (exp[0] === 'var')
        {
            const [_, name, value] = exp;
            return env.define(name, this.eval(value, env));
        }

        if (isVariableName(exp))
        {
            return env.lookup(exp);
        }

        // Block
        if (exp[0] === 'begin')
        {
            // new block -> new environment
            const blockEnv = new Environment({}, env);

            return this._evalBlock(exp, blockEnv);
        }

        throw 'unimplemented : ${JSON.stringfy(exp)}';
    }

    _evalBlock(block, blockEnv)
    {
        let result;

        const [_tag, ...expressions] = block;

        expressions.forEach(exp=>{
            result = this.eval(exp, blockEnv)
        })

        return result;
    }
}

function isNumber(exp)
{
    return typeof exp === "number";
}

// "" 안에 있는 값을 리턴하도록 할 것이다.
function isString(exp)
{
    return typeof exp === 'string' && exp[0] === '"' && exp.slice(-1) === '"';
}

function isVariableName(exp)
{
    // 변수는 알파벳으로 시작, 끝나는 것도 알파벳 혹은 숫자 
    return typeof exp === 'string' && /^[a-zA-Z][a-zA-Z0-9_]*$/.test(exp);
}


// test : Math
const eva = new Eva(new Environment(
    {
        // 기본 global variable 정의해두기
        null : null,
        true : true,
        false : false,
        VERSION : 0.1
    }
));
assert.strictEqual(eva.eval(1), 1);
assert.strictEqual(eva.eval('"hello"'), 'hello'); 
assert.strictEqual(eva.eval(['+', ['+', 3, 2], 5]), 10);
assert.strictEqual(eva.eval(['*', ['+', 3, 2], 5]), 25);

// variables
assert.strictEqual(eva.eval(['var', 'x', 10]), 10)
assert.strictEqual(eva.eval('x'), 10)
// assert.strictEqual(eva.eval('y'), 10)
assert.strictEqual(eva.eval('VERSION'), 0.1)

// var isUser = true
assert.strictEqual(eva.eval(['var', 'isUser', 'true']), true)
assert.strictEqual(eva.eval(['var', 'z', ['*', 2, 2]]), 4)
assert.strictEqual(eva.eval('z'), 4)

// Blocks
assert.strictEqual(eva.eval(
    ['begin',
        ['var', 'x', 10],
        ['var', 'y', 20],
        ['+', ['*', 'x', 'y'], 30],
    ]
), 230)

assert.strictEqual(eva.eval(
    ['begin',
        ['var', 'x', 10],
        ['begin',
            ['var', 'x', 20],
            'x'
        ],
        'x'
    ]
), 10)

console.log("all assertions passed")