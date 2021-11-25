/*
프로세스의 상태 3가지
- st, ready, running, blocked, ed

Running : cpu에 의해 프로세스가 실행중인 상태
cpu의 코어가 하나라는 것은, Running 상태에 있는 프로세스의 개수가 최대 1개 라는 의미이다. 

Ready : Running 하고 있는 상태
- 즉, 프로세스는 모든 준비가 끝난 상태
- 그런데 Running 상태에 있는 애 때분에 실행되지 못하는 것
- Ready 에서 Running으로 옮겨주는 것이 바로, 프로세스 스케줄링이다.

Q. 어플리케이션 더블 클릭 시 일어나는 현상 ---

바탕화면에서 어플리케이션을 더블클릭한다고 바로 Running 상태가 되는 것이 아니다
Ready 상태가 되고, 프로세스 스케줄러가, 자기 자신을 선택할 때까지 기다린다. 

cpu와 관련하여 연산은 2가지로 나뉜다.
cpu에 의존적인 연산 ex) 사칙연산
cpu에 의존적이지 않은 연산 ex) I/O

두 연산은 병행할 수 있다.
C 프로세스가 어떤 데이터를 읽어오는 중이다.
File
그런데 시간이 오래 걸린다.
이와 같이 I/O 를 수행중이라면
그 동안 다른 프로세스가 실행되게 하는 것이 좋다.

---
I/O 바운드를 위핸 다른 프로세스에게 양보해 줄 때는
Running 상태에서, Blocked 상태로 들어단다.

I/O가 끝나면 Ready 상태로 들어간다 

반면, 순전히 다른 프로세스에게 공평한 시간 분배를 위해
양보할 때는 Running 상태에서 Blocked 상태로 들어간다.




---
*/



