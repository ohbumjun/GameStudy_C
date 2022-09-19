#pragma once

/*
> CP 오브젝트 생성

HANDLE WINAPI CreateIoCompletionPort(
  _In_     HANDLE    FileHandle,    // CP 오브젝트 생성시 INVALID_HANDLE_VALUE 전달
  _In_opt_ HANDLE    ExistingCompletionPort, // CP 오브젝트 생서이 NULL 전달
  _In_     ULONG_PTR CompletionKey, // CP 오브젝트 생성시 0 전달   
  _In_     DWORD     NumberOfConcurrentThreads // CP 오브젝트에 할당되어 완료된
                                               // IO를 처리할 쓰레드 수 전달
                                               // ex) 2를 전달하면 CP 오브젝트에
                                               // 할당되어 동시 실행 가능한 
                                               // 쓰레드의 수가 2개로 제한
                                               // 0 전달하면, 시스템의 CPU개수가
                                               // 동시 실행 가능한 쓰레드의
                                               // 최대수로 지정 (코어 개수)
);

> CP 오브젝트 연결

HANDLE WINAPI CreateIoCompletionPort(
  _In_     HANDLE    FileHandle, // CP 오브젝트에 연결할 소켓 핸들 전달
  _In_opt_ HANDLE    ExistingCompletionPort, // 소켓과 연결할 CP 오브젝트 핸들 전달
  _In_     ULONG_PTR CompletionKey, // 완료된 IO 관련 정보의 전달을 위한 매개변수
  _In_     DWORD     NumberOfConcurrentThreads //  2번째 매개변수가 NULL 이 아니면 무시
);

> Completion Port 의 완료된 IO 확인과 쓰레드의 IO 처리
- IO를 전담하는 쓰레드가 IO 의 완료를 확인하기 위해서 해당 함수를 호출
- 단, 실헤 IO 의 완료에 대한 응답을 받는 쓰레드의 개수는 CreateIoCompletionPort 호출시 지정한 최대 쓰레드의 수를 넘지 않는다.

BOOL GetQueuedCompletionStatus(
  [in]  HANDLE       CompletionPort, // 완료된 IO 정보가 등록되어 있는 CP 오브젝트 핸들 전달

        LPDWORD      lpNumberOfBytesTransferred, // 입출력 과정에서 송수신된 데이터의
                                                 // 크기정보를 저장할 변수의 주소값 전달

  [out] PULONG_PTR   lpCompletionKey,            // CreateIoCompletionPort 의 3번째 인자로 전달된
                                                 // 값을 저장을 위한 변수의 주소값 전달

  [out] LPOVERLAPPED *lpOverlapped,              // WSASend, WSARecv 함수 호출시 전달하는
                                                 // OVERLAPPED 구조체 변수의 주소값이 저장될 변수 주소값 전달

  [in]  DWORD        dwMilliseconds              // 타임아웃 정보 전달, 여기서 지정한 시간이 완료되면 FALSE를 반환하면서 함수를 빠져나가며, 
                                                 // INFINITE 를 전달하면 완료된 IO가 CP 오브젝트에 등록될 때까지 블로킹 상태에 있게 된다.
);

즉, GetQueuedCompletionStatus 함수의 3번째 인자를 통해서 얻게 되는 것은 소켓과 CP 오브젝트의 연결을 목적으로 
CreateIoCompletionPort 함수가 호출될 때 전달되는 3번째 인자값

GetQueuedCompletionStatus 함수의 4번째 인자로 얻게되는 것은 WSASend, WSARecv 함수호출시 전달된는 WSAOVERLAPPED 구조체 변수의 주소값

*/