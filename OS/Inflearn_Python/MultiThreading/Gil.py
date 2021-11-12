'''
Section 2
Multithreading - Python's GIL

Keyword - CPython, 메모리 관리, GIL 사용 이유
'''

'''
Gil(Global Interpreter Lock) --- 

(1). CPython --> Python(bytecode) 실행 시 여러 thread 사용할 경우
    단일 스레드 만이 Python object에 접근하게 제한하는 mutex
(2). CPython 메모리 관리가 취약하다
(3). 단일 스레드로 충분히 빠르다
(4). 멀티 프로세스 사용은 가능 ex) numpy
(5). 병렬 처리는 Multiprocessing, async io 선택지 다양하다.
(6). Thread 동시성을 완벽 처리를 위해 (즉, GIL이 아닌, 우회하기) --> Jython, IronPython, Stackless Python
    등이 존재한다 

< 해설 >
작성 코드.py --> 내부적으로 CPython이 해석 --> Python(byte) 코드로 바뀐다 
--> Python(bytecode) 실행 시, 여러 thread 사용할 경우 
멀티 쓰레드로 사용하더라도, 한 타임에는 한 쓰레드만이
python object 에 접근하게끔 제한해둔 것

- Single Thread로 사용할 때, 성능이 가장 빨랐다.


'''
