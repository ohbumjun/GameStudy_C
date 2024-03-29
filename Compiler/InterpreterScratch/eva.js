const assert = require('assert');
const Environment = require('./environment');
const Transformer = require('./transform/transformer')
const evaParser = require('./parser/evaParser')
const fs = require('fs')

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
    constructor(global = GlobalEnvironement)
    {
        this.global = global;
        this._transformer = new Transformer();
    }

    /*
    begin 형태로 자동으로 감싸준다.
    */
    evalGlobal(expression)
    {
        // return this._evalBlock(['block', expression], this.global);
        return this._evalBody(expression, this.global);
    }
    eval(exp, env = this.global)
    {
        // console.log(`exp : ${exp}`)

        if (this._isNumber(exp))
        {
            return exp;
        }
        if (this._isString(exp))
        {
            return exp.slice(1, -1);
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
        // Variables ----------------
        if (exp[0] === 'var')
        {
            const [_, name, value] = exp;

            // console.log(`var => name, value : ${name}, ${value}`)

            return env.define(name, this.eval(value, env));
        }

        if (this._isVariableName(exp))
        {
            return env.lookup(exp, env);
        }

        // Block ----------------
        if (exp[0] === 'begin')
        {
            // new block -> new environment
            const blockEnv = new Environment({}, env);

            return this._evalBlock(exp, blockEnv);
        }

        // assign ----------------
        if (exp[0] === 'set')
        {
            // ref : 변수 이름 혹은 instance property
            const [_, ref, value] = exp;

            if (ref[0] == 'prop')
            {
                // ex) (set (prop this x) x)
                //     _ref     : (prop this x)
                //     _tag     : prop
                //     instance : this
                //     propName : x
                //     value    : x
                const [_tag, instance, propName] = ref;

                // eX) (prop this x) => this 라는 이름의 "변수" 형태로 등록된 environment 를 가져온다.
                const instanceEnv = this.eval(instance, env);

                // console.log(`prop ref : ${ref}`)
                // console.log(`prop instance Env: ${instanceEnv}`)
                // console.log(`propName, value : ${propName}, ${value}`)

                // instance 만의 environment 에 'x' 라는 변수에 대해 인자 x 를 정의한다. 
                return instanceEnv.define(
                    propName,
                    this.eval(value, env)
                );
            }

            return env.assign(ref, this.eval(value, env));
        }

        // if ----------------
        if (exp[0] == 'if')
        {
            const [_tag, condition, consequent, alternate] = exp;
            if (this.eval(condition, env))
            {
                return this.eval(consequent, env);
            }
            return this.eval(alternate, env);
        }

        // while ----------------
        if (exp[0] == 'while')
        {
            const [_tag, condition, body] = exp;
            let result;
            while (this.eval(condition, env))
            {
                result = this.eval(body, env);
            }
            return result;
        }

        // lambda function -----------------
        // (lambda (x) (* x x))
        if (exp[0] == 'lambda')
        {
            const [_tag, params, body] = exp;

            // params.forEach((b) => {console.log(`lambda param : ${b}`)})
            // body.forEach((b) => {console.log(`lambda body : ${b}`)})

            // return function
            return {
                params,
                body,
                env,   // Closure
            };
        }

        // switch -----------------------------
        // Syntatic sugar for nested if-expression
        if (exp[0] == 'switch')
        {
            const ifExp = this._transformer.transformSwithToIf(exp);

            return this.eval(ifExp, env);
        }

        // class declaration ex) class <Name> <Parent> <Body>
        if (exp[0] == 'class')
        {
            console.log("class made")

            const [_tag, name, parent, body] = exp;

            // ex) class Point null : parentEnv 와 env (=global) 는 같게 된다. (base class 를 생성할 경우)
            const parentEnv = this.eval(parent, env) || env;

            const classEnv = new Environment({}, parentEnv);

            this._evalBody(body, classEnv);

            // environement 에 변수 형태로 저장한다.
            // ex) key == Point : val == Pointe Env
            return env.define(name, classEnv);
        }

        // super expression : (super <className>)
        if (exp[0] == 'super')
        {
            const [_tag, className] = exp;
            return this.eval(className, env).parent;
        }

        // class instantiation
        if (exp[0] === 'new')
        {
            // ex) new Point 20 10 : Point Class 에 해당하는 "Environment" 가 "변수" 형태로
            //     저장되어 있으므로, 해당 class Environment 를 가져온다.
            //     해당 class envrionment 안에는  constructor, calc 등의 멤버 함수 및
            //     멤버 변수들이 또, record 에 변수 형태로 저장되어 있다.
            const classEnv = this.eval(exp[1], env)

            // parent component of instance environment is set to its class
            // 즉, class Env 를 부모 env 로 하여, 새로운 env 를 만들어낸다.
            const instanceEnv = new Environment({}, classEnv)

            // ex) new Point 20 10 => 생성자 인자 20,10 을 가져온다.
            const args = exp.slice(2).map(arg => this.eval(arg,env))

            // call constructor
            // ex) constructor (this x y) 에서 this 는 instance 의 Environment 에 대응될 것이다. 
            this._callUserDefinedFunction(
                classEnv.lookup('constructor'),
                [instanceEnv, ...args]
            );

            return instanceEnv;
        }

        // Property access : instance -> class 에 있는 멤버함수 및 멤버 변수 등 정보 접근
        // - prop <instsance> <name>
        if (exp[0] == 'prop')
        {
            const [_tage, instance, name] = exp;

            const instanceEnv = this.eval(instance, env);

            return instanceEnv.lookup(name);
        }

        // import module ---- : import <name>
        if (exp[0] == 'import')
        {
            // module source code 를 import 해야 한다.
            // 이를 위해서는 source code 를 parse 해야 한다.
            const [_tag, name] = exp;

            const moduleSrc = fs.readFileSync(
                `${__dirname}/modules/${name}.eva`,
                'utf-8'
            );

            const body = evaParser.parse(`(begin ${moduleSrc})`);

            const moduleExp = ['module', name, body];

            // 참고 : 이미 module 이 import 된 적이 있다면, cache 과정을 적용한다.
            // 이렇게 다시 parse 하는 과정을 거치는 것이 아니라, 이미 cache 에 존재하는
            // module 을 바로 리턴해주는 것이다.

            return this.eval(moduleExp, this.global);
        }

        // module declaration --- : (module <body>)
        if (exp[0] == 'module')
        {
            const [_tag, name, body] = exp;

            // module 의 env 는 module 이 만들어진 context 상의 env 로 세팅해준다.
            const moduleEnv = new Environment({},env);

            this._evalBody(body, moduleEnv);

            return env.define(name, moduleEnv);
        }
 
        // function declaration ----------------
        if (exp[0] == 'def')
        {
            // JIT-transpile to variable declaration : create variable expression node directly at runtime
            // 즉, JIT 의 경우, 함수 선언을 만나면, 런타임 동안에 lambda 함수를 만들어내는 역할을 하는 것이다.
            const varExp = this._transformer.transformDefToLambda(exp);

            return this.eval(varExp, env);
            /*
            const fn = {
                params,
                body,
                // Closure 개념을 적용할 것이다.
                // 즉, 해당 함수가 존재하는 Environment 도 같이 인자로 받아서
                // 외부 Environment 에 존재하는 function 및 변수의 존재도 인지할 수 있게 한다는 것이다.
                env
            };
            return env.define(name, fn);
            */
        }

        // function call ----------------
        // ex) (print "Hello world")
        //     (+ x 5)
        //     (> foo bar)
        if (Array.isArray(exp))
        {
            console.log(`array exp : ${exp}`)

            // [ '*', 'x', 'y' ] 에서 '*' 에 해당하는 function 을 리턴할 것이다.
            // 그리고 해당 function 은 GlobalEnvironment 를 초기화 할 때, 변수 형태로 세팅했었다.
            const fn = this.eval(exp[0], env);

            // exp 에서 argument 를 뽑아내서, 각각의 argument 를 evaluate 할 것이다.
            // 즉, fn 를 실제 실행하기 전에 argument 각각에 대해서 평가할 것이라는 의미이다.
            const args = exp.slice(1).map(arg => this.eval(arg, env));

            /*1. JS Native Function*/
            if (typeof fn == 'function')
            {
                return fn(...args);
            }

            /*2. User - Defined*/
            return this._callUserDefinedFunction(fn, args)
        }

        throw `Unimplemented: ${JSON.stringify(exp)}`;
    }

    _callUserDefinedFunction(fn, args)
    {
        // actual storage for local variable and parameter
        const activationRecord = {};

        fn.params.forEach((param, index) => {
            // console.log(`param : ${param}`)
            // console.log(`param body : ${args[index]}`); 
            activationRecord[param] = args[index];
        })
        // console.log(`func body : ${fn.body}`)

        // 함수 == 또 다른 Envrironment 를 가진다. 
        // closure 개념을 위해 parent environment 를 세팅
        // https://www.notion.so/7-23-2c387e4c038842f79d1df2c6df6d8d85
        // env    : dynamic scope == environment which we are called
        // fn.env : static scope  == captured environment
        const activationEnv = new Environment(activationRecord, fn.env);

        // fn.body.forEach((b) => {console.log(`found body : ${b}`)})

        return this._evalBody(fn.body, activationEnv);
    }

    _evalBody(body, env)
    {
        // body.forEach((b) => {console.log(`execute body : ${b}`)})

        if (body[0] == 'begin')
        {
            return this._evalBlock(body, env);
        }
        
        return this.eval(body, env);
    }

    _evalBlock(block, blockEnv)
    {
        // block.forEach((b) => {console.log(`execute block : ${b}`)})
        let result;

        const [_tag, ...expressions] = block;

        expressions.forEach(exp=>{
            // console.log(`exp in Block : ${exp} `)
            result = this.eval(exp, blockEnv)
        })

        return result;
    }

    _isNumber(exp)
    {
        return typeof exp === "number";
    }

    // "" 안에 있는 값을 리턴하도록 할 것이다.
    _isString(exp)
    {
        return typeof exp === 'string' && exp[0] === '"' && exp.slice(-1) === '"';
    }

    _isVariableName(exp)
    {
        // console.log(`isvariable : ${exp}`)

        // 1) 변수는 알파벳으로 시작, 끝나는 것도 알파벳 혹은 숫자 
        // 2) +, - 등의 함수들을 Global Environment 에 "변수" 형태로 넣어둔 상태이다.
        return typeof exp === 'string' && /^[+\-*/<>=a-zA-Z_][a-zA-Z0-9_]*$/.test(exp);
    }
}

/*
Default Global Environment
*/
const GlobalEnvironement = new Environment(
    {
        // 기본 global variable 정의해두기
        null : null,
        true : true,
        false : false,
        VERSION : 0.1,

        /*
        편의를 위해 Global Environment 에 기본적인 연산 함수를 "변수" 로 둔다.

        즉, eva._isVariable(exp) 에서 true 를 리턴하게 되는 것이다.

        만약 해당 함수를 다른 방식으로 사용하고자 한다면
        Sub Environment 들에서 해당 연산 함수를 override 하게 한다.

        ex) (+ ratio 30)
        */
        '+' (op1, op2) {
            return op1 + op2;
        },

        '-' (op1, op2 = null) {
            if (op2 == null)
            {
                return -op1;
            }
            return op1 - op2;
        },

        '*' (op1, op2) {
            return op1 * op2;
        },

        '/' (op1, op2) {
            return op1 / op2;
        },

        // Comparison
        '=' (op1, op2) {
            return op1 === op2;
        },
        '!=' (op1, op2) {
            return op1 != op2;
        },
        '>' (op1, op2) {
            return op1 > op2;
        },
        '<' (op1, op2) {
            return op1 < op2;
        },
        '>=' (op1, op2) {
            return op1 >= op2;
        },
        '<=' (op1, op2) {
            return op1 <= op2;
        },

        // Console output
        print(...args)
        {
            console.log(...args);
        }
    }
);

module.exports = Eva;