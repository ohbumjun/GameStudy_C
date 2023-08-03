const assert = require('assert')
const testUtil = require('./test-util.js')

module.exports = eva =>
{
    assert.strictEqual(eva.eval(['var', 'isUser', 'true']), true)
    assert.strictEqual(eva.eval(['var', 'z', ['*', 2, 2]]), 4)
    assert.strictEqual(eva.eval('z'), 4)

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
    
    
    assert.strictEqual(eva.eval(
        ['begin',
            ['var', 'val', 10],
            // 밖 environment 로의 접근
            ['var', 'result', 
                ['begin',
                    ['var', 'x', ['+', 'val', 10]],
                    'x'
                ],
            ],
            'result'
        ]
    ), 20)
    
    // s-expression 형태의 string 을 인자로 받을 것이다.
    testUtil.test(eva, `
        (begin
            (var x 10)
            (var y 20)
            (+ (* x 10) y)
        )
    `, 120);
}