/*
Instruction set for eva VM

- (참고 : bytecode 형태로 code 가 변환되고, 그 bytecode 로 코드를 실행하는 것이라면
          byte 별로 의미가 다 다르게 다르다는 것이고, 어떤 byte 가 어떤 의미인지에 대한
          대응관계가 존재하는 것인가? )
*/
#ifndef __OpCode_h

#define __OpCode_h

/*
Stop Program
- Prograrm 을 중지하라는 의미의 bytecode 는 0 이다.
*/
#define OP_HALT 0x00

/*
Push a const onto stack
*/
#define OP_CONST 0x01

/*
Math operation
*/
#define OP_ADD 0x02
#define OP_SUB 0x03
#define OP_MUL 0x04
#define OP_DIV 0x05

#endif