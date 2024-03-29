const assert = require('assert')
const {test} = require('./test-util')

module.exports = eva =>
{
    test(eva, `
        (begin
            (def square (x)
                (* x x))
            (square 2)
        )
    `, 
    4);

    test(eva, `
        (begin
            (def calc (x y)
                (begin
                    (var z 30)
                    (+ (* x y) z)
                ))
            (calc 10 30)
        )
    `, 
    330);

    test(eva, `
        (begin
            (var value 100)
            (def calc (x y)
                (begin
                    (var z (+ x y))
                    (def inner (foo)
                        (+ (+ foo z) value))
                    inner
                ))
            (var fn(calc 10 20))

            (fn 30)
        )
    `, 
    160);
};