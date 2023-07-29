const assert = require('assert');

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
            return this.eval(exp[1]) + this.eval(exp[2]);
        }
        if (exp[0] === '*')
        {
            return this.eval(exp[1]) * this.eval(exp[2]);
        }
        if (exp[0] === '+')
        {
            return this.eval(exp[1]) + this.eval(exp[2]);
        }
        if (exp[0] === '*')
        {
            return this.eval(exp[1]) * this.eval(exp[2]);
        }
        if (exp[0] === '-')
        {
            return this.eval(exp[1]) - this.eval(exp[2]);
        }
        if (exp[0] === '/')
        {
            return this.eval(exp[1]) / this.eval(exp[2]);
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
            return env.define(name, value);
        }

        throw 'unimplemented : ${JSON.stringfy(exp)}';
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

// test : Math
const eva = new Eva();
assert.strictEqual(eva.eval(1), 1);
assert.strictEqual(eva.eval('"hello"'), 'hello'); 
assert.strictEqual(eva.eval(['+', ['+', 3, 2], 5]), 10);
assert.strictEqual(eva.eval(['*', ['+', 3, 2], 5]), 25);

// variables


console.log("all assertions passed")