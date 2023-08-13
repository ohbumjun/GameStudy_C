const assert = require('assert')

module.exports = eva =>
{
    assert.strictEqual(eva.eval(
        ['begin',
            ['var', 'x', 20],
            ['var', 'y', 0],

            ['if', ['>', 'x', 10],
                // if 가 true
                ['set', 'y', 20],
                // else
                ['set', 'y', 30]
            ],
            'y'
        ]
    ), 20)
}