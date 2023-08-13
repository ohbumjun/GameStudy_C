const assert = require('assert')
const {test} = require('./test-util')

module.exports = eva =>
{
    test(eva, `(+ 1 5)`, 6);
    test(eva,`(+ (+ 2 3) 5)`, 10);

    // comparison
    test(eva,`(> 1 5)`, false);
}