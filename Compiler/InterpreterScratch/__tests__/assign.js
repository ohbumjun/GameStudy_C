const assert = require('assert')

module.exports = eva =>
{
    assert.strictEqual(eva.eval(
        ['begin',
            ['var', 'data', 10],
            ['begin',
                ['set', 'data', 20]
            ],
            'data'
        ]
    ), 20)
    
}