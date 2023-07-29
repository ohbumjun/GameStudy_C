const Eva = require('../eva');
const Environment = require('../environment');


// test 내용을 한꺼번에 모아서 실행한다.
const tests = [
    require('./assign.js'),
    require('./block.js'),
    require('./variable.js'),
    require('./basic_operation.js')
];

const evaInterpreter = new Eva(new Environment(
    {
        // 기본 global variable 정의해두기
        null : null,
        true : true,
        false : false,
        VERSION : 0.1
    }
));

tests.forEach(test=>test(evaInterpreter))

console.log("all assertions passed")