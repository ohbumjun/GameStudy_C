/*
Q. 프로세스 ?

- 메인 메모리로 이동하여, 실행중인 프로그램

Q. 프로세스의 범위 

- 메모리 구조 + 레지스터 set

기본적으로 프로세스 하나를 생성을 하면
Code 영역 = 명령어
Data 영역 = 전역 변수, Static 변수
Heap 영역 = 동적 할당
Stak 영역 = 지역 변수, 전달 인자 정보 

이러한 메모리 공간이 할당이 된다. 

그런데
256 mb 의 메모리만 가지고 있는데
만일 4g 의 메모리가 필요한 메모리 공간을 할당 받았다면 ?

즉, 램보다 더 큰 메모리 공간을 할당받았다면 ?
램에 실제로 위의 모든 메모리 공간이 올라갈 수 없다
하드 디스크 까지 영역을 할당해야 한다.

일단, 가상 메모리의 도움으로
위의 메모리가 할당되었다가 생각을 해보자. 
--
또한 프로그램이 실행된다 ?
cpu 에서 프로그램을 실행한다.
왜냐하면 여기에 alu 라는 연산장치가 있기 때문이다.
뿐만 아니라 ALU의 연산을 도와주는 
레지스터 set 도 있다.

레지스터는, 현재 실행중인 프로세스에 대한 정보들로
정보가 가득채워진다.

ex) A 프로세스 실행 --> A 프로세스에 대한 정보가 레지스터에
그러다가 만약 B 프로세스가 실행되면,
기존에 레지스터가 저장하고 있던 A프로세스에 대한 정보는
다른 메모리 set에 저장을 해두고
B 프로세스에 대한 정보가 레지스터를 채운다 .

프로세스 별로,
독립적인 대상
즉, 프로세스가 A -> B, B -> A 바뀔 때
프로세스 별, 독립적인 대상은, 
프로세스에 종속적인 대상은
무조건 프로세스의 범주에 포함시킬 수 있다는 것이다.

Q. 프로세스 재정의  ----

프로세스를 구성하는 범주는,
프로세스 별로 독립적으로 할당받는 리소스다
ex) 메모리 공간, 가상 메모리 공간, 프로세스 별로 
독립적으로 사용하게 되는 레지스터 set도 
프로세스 범주에 속하게 된다.

Q. Context Switching ---
ex) A 프로세스 실행 --> A 프로세스에 대한 정보가 레지스터에
그러다가 만약 B 프로세스가 실행되면,
기존에 레지스터가 저장하고 있던 A프로세스에 대한 정보는
다른 메모리 set에 저장을 해두고, 레지스터를 비운 다음
B 프로세스에 대한 정보가 레지스터를 채운다 .

이러한 과정을 Context Switching 이라고 하는 것이다.

Q. 프로세스 스케줄러 --- 

- 프로세스를 스케줄링 하는 것이다
cpu는 하나이다. 그런데 여러개의 프로세스를 실행 시킨다.
cpu는 한번에 한개의 프로세스만을 실행시킨다.
프로세스 스케줄링은, 프로세스의 순서를 정해줘서
공평하게 cpu에 의해 실행될 수 있도록 관리해주는 것 

- cpu 입장에서는, 프로세스 스케줄러도 프로세스이다.
즉, 스케줄러가 동작하고 있을 때
다른 프로세스들이 일하지 못하고 있을 수도 있다는 것이다. 

Q. 프로세스의 상태 3가지 ---

St, Ready, Running, Blocked, End

프로세스라는 것은, 상태 정보를 지닌다.
Ready, Running, Blocked 상태

Running : cpu에 의해 실행중인 상태
cpu 의 코어가 하나라는 것은, running 상태가 될 수 있는
프로세스는 한개가 최대

Ready : Running 하고 싶은 상태
- 즉, 프로세스는 모든 준비가 끝난 상태 
- 그런데 Running에 있는 애 때분에 실행 못하는 것이다
- Ready에서 Running으로 옮겨주는 것이 바로 프로세스 스케줄러이다.

Q. 어플리케이션 더블 클릭 시 일어나는 현상 ------------

ex) 
1번째 Case --- 
우리가 어떤 어플리케이션을 더블 클릭
더블 클릭한다고 곧바로 실행상태가 되는 것이 절대 아니다
내가 직접 나의 상태를 Running으로 만들 수 없고
반드시 스케줄러에 의해 선택되어야 하기 때문이다.

따라서 클릭하면, 곧바로 REady 상태가 되고
스케줄러가 선택되기를 기다려야 한다.


Q. Ready ~ Running ~ Blockec Change 2가지 경우 


2번째 Case --- 
A가 Ready, C가 Running 상태

연산 중에는
CPU의 ALU에 의존적인 연산이 있다
ex) 사칙연산 등
CPU에 의존적이지 않은 연산이 있다
ex) I/O 연산- File, Sound, Graphic .... 

즉, I/O 연산과 cpu 에 의한 사칙연산 등은
병행할 수 있다는 것이다

C 라는 프로세스가 I/O 연산을 수행 중이다.
서버에 접속해서 데이터를 가져오고 있다고 해보자.
그런데 너무 양이 크다.

지금 I/O 연산을 하고 있으니까 
C는 잠깐 쉬고, 다른 프로세스가 돌아가게 하는 것이 좋다.

이 경우, 즉, I/O 연산이 끝날 때까지 기다리고
다른 프로세스에 양보하는 Case 에는
C, 즉, Running 중이던 프로세스가
Blocked 상태로 들어가게 된다. 

즉, I/O 연산을 하는 경우에는 Blocked 상태가 된다. 
I/O 연산이 끝나면 다시 Ready 상태로 들어간다. 

+++
반면, A,B,C 라는 프로세스가 있다.
사용자 관점에서는 동시에 사용되는 것처럼 보인다
Cpu가 아주 고속으로 A,B,C 를 번갈아 가면서 실행하는 것

C만 계속 실행 하는 것이 아니라
A,B 도 실행 시켜주어야 한다.
따라서, A에게 Running을 양보하고, 
자기는 Ready로 

즉, I/O를 위해 양보하는 경우는 Blocked 로
그게 아니라, 어쩔 수 없이 양보하는 경우에는
Ready 상태로 되돌아가게 한다. 

*/