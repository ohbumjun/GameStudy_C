const assert = require('assert')

module.exports = eva =>
{
    assert.strictEqual(eva.eval(1), 1);
    // '""' 형태로 사용하게 되면, string 형태가 된다.
    // 반면 그저 '' 형태만 변수를 의미한다.
    assert.strictEqual(eva.eval('"hello"'), 'hello'); 
    assert.strictEqual(eva.eval(['+', ['+', 3, 2], 5]), 10);
    assert.strictEqual(eva.eval(['*', ['+', 3, 2], 5]), 25);
}