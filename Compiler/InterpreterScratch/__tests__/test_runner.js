const Eva = require('../eva');
const Environment = require('../environment');

// test 내용을 한꺼번에 모아서 실행한다.
const tests = [
    // require('./assign.js'),
    // require('./block.js'),
    // require('./variable.js'),
    // require('./basic_operation.js'),
    // require('./if_test.js'),
    // require('./while.js'),
    // require('./built-in-function-test.js'),
    // require('./user-defined-function-test.js'),
    // require('./lambda-function-test.js'),
    // require('./switch-test.js'),
    // require('./class-test.js'),
    require('./module-test.js'),
];

const evaInterpreter = new Eva();

tests.forEach(test=>test(evaInterpreter));

console.log("all assertions passed");