// Q. 32 vs 64 bit 구분 기준 2개 ? ---

// 한번에 송수신 가능한 데이터 크기
// - io bus에 의존적 

// 데이터 처리 능력 
// - cpu
// - cpu가 한번에 읽어들일 수 있는 명령어 크기

// 즉, 64bit 라고 완벽하게 얘기하기 위해서는
// - 데이터가 i/o bus에 64bit 크기로 전달될 수도 있어야 하고
// - cpu도 64bit 를 읽어들여서 명령어를 처리해야 한다
// 둘다 64bit 여야만 한다는 것이다

// Q. 32 --> 64 bit 의 의미 ? -------------------------------------

// 프로그램으로 표현할 수 있는 범위의 증가
// 표현할 수 있는 메모리의 전체 크기 

// Q. 각각에서의 포인터 크기, 포인터 크기의 의의 ? -------------------

// 일반적으로 32bit 시스템에서는 포인터가 32bit
// 64bit 시스템에서는 포인터도 64bit 를 사용한다.
// 왜 그런 것일까 ?
// 즉, 각각 4byte, 8byte 가 된다.

// 포인터가 크면 클수록, 프로그래머에게 유리하다
// 포인터가 크다는 것은, 우리가 표현가능한 주소의 값이 크다는 것이다
// 포인터가 크다는 것은, 메모리에 접근할 수 있는
// 범위가 그만큼 크다는 것을 의미하기 때문이다 

// 그런데, 포인터 크기를 고려하는 데에 있어서
// I/O Bus 크기도 같이 고려해야 한다.
// 포인터 크기는 128 로 잡고, I/O Bus 크기는 64로 잡았다면 ? 
// 64 * 2 = 128
// 총 2번에 걸쳐서 정보를 전달할 수 밖에 없다는 것이다
// 이는 시스템의 전반적인 성능저하를 가져오게 된다.

// 하나의 주소를 사용하기 위해서
// IO Bus를 2번 사용해야 한다.
// 상당한 성능 저하이다.

// 일단 메모리는 크면 클 수록 좋되
// 버스에서 한번에 전송할 수 있는 크기만큼 !
// ----------------------------------------------------------------

// 메모리가 크다는 것은, 메인 메모리를 의미한다
// 맨 앞, 혹은 매뒤, 모두 한번에 address 할수 있도록
// 전체를 address 할 수 있도록, 전체 pointer의 크기가 충분히 커야 한다는 것이다

// 32bit 시스테멩서는 한번의 access할 수 있는 메모리 크기
// 2^32 == 4GB

// 64비트 시스템 같은 경우에는
// 주소를 표현하는데 있어서, 64bit로 표현한다
// 왜 ? 이것이 최선이므로
// 가급적, 주소값은 넓게 표현하고
// 한번에 보낼 수 있어야할 것이다.
// 64bit 에서는 표현할 수 있는 주소의 범위가 2^64가 될 것이다


// 정확히 0 번지부터 2^64 -1 까지 표현가능하다
// 매우 큰 범위이다

// 즉, 32bit 시스템에서 64로 넘어갔다는 의미는
// " 우리가 프로그램 상에서 활용할 수 있는
// 최대 메모리의 크기가 증가했음을 의미한다. "

// 메인메모리는 내가 256mb인데
// 시스템이 64bit라면, 의미없자나
// 메인 메모리 확장 못하면....?

// 이 사이에 어떤 관계
// 가상 메모리 virtual memory를 통해 극복한다.