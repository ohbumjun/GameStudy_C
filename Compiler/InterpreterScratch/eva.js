const assert = require('assert');

/*
* eva interpreter
- ast 를 받아서, 실제 실행을 해주는 녀석
- ast 는 single 형태로 구성될 것이다.
*/
class Eva
{
    eval(exp)
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