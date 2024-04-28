#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);
	UINT pageSize = sysInfo.dwPageSize;

	// 1. 힙 생성
	HANDLE hHeap = HeapCreate(
		// - 생성되는 힙의 특성 부여
		// - HEAP_GENERATE_EXCEPTION : 오류 발생시 NULL 반환 하지 않고
		//                                                  예외 발생
		// - HEAP_NO_SERIALIZE             : 생성된 힙에 메모리 할당 시
		//                                                 동기화 처리 하지 않게 된다 (동기화문제에서 자유로워짐)
		//    쓰레드 별로, 독립된 힙을 할당할 경우 사용
		HEAP_NO_SERIALIZE,

		// - 3번째 인자로 지정한 메모리 중에서, 초기에 할당할 메모리 크기
		// - 해당 페이지 수만큼 Commit 상태가 된다.
		pageSize * 10,

		// - 생성되는 힙의 크기
		// - 지정하는 크기에 해당하는 페이지 수 만큼 Reserve 상태가 된다.
		// - 0 이 전달될 경우 힙은 증가 가능한 메모리가 된다. 
		//   즉, 0이 전달되면, 메모리가 허락하는 한도 내에서 힙의 크기 증가
		pageSize * 100
	);	

	// 2. 메모리 할당
	int* p = (int*)HeapAlloc(
		hHeap, 

		// - HEAP_GENERATE_EXCEPTION : 오류 발생시 NULL 반환 하지 않고
		//                                                  예외 발생
		// - HEAP_NO_SERIALIZE             : 생성된 힙에 메모리 할당 시
		//                                                 동기화 처리 하지 않게 된다 (동기화문제에서 자유로워짐)
		//    쓰레드 별로, 독립된 힙을 할당할 경우 사용
		//    단, HeapCreate 시에 전달했다면, 중복 지정 필요 X
		// - HEAP_ZERO_MEMORY : 할당된 메모리는 0 으로 초기화
		0, 

		// - 할당하고자 하는 메모리 크기
		sizeof(int) * 10);

	// 3. 메모리 활용
	for (int i = 0; i < 10; ++i)
		p[i] = i;

	// 4. 메모리 해제
	HeapFree(
		hHeap,

		// HEAP_NO_SERIALIZE 가 올 수 있다.
		// - HeapCreate 시에 전달했다면, 중복 지정 필요 X
		0, 

		// 해제할 메모리 시작 주소
		p
	);

	// 5. 힙 소멸
	HeapDestroy(hHeap);

	// 디폴트 힙을 얻어올 때 사용하는 함수
	HANDLE hDefaultHeap = GetProcessHeap();

	// 아래의 과정은, malloc, free 함수를 통해
	// 디폴트 힙에 메모리를 할당하고 반환하는 것과 동일
	TCHAR* pDefault = (TCHAR*)HeapAlloc(
		hDefaultHeap,
		HEAP_NO_SERIALIZE,
		sizeof(TCHAR) * 30
	);

	// 그러면, 해당 함수와 (malloc, free) 의 차이 ?
	// - 추가적인 컨트롤이 가능하다는 것이다.
	// - HEAP_NO_SERIALIZE : 동기화 처리를 하지 않는 것
	// - 어차피, 독립적인 힙 영역을 할당하기 때문에 !

	_tcscpy(pDefault, _T("Default Heap!"));
	_tprintf(_T("%s \n"), pDefault);

	HeapFree(hDefaultHeap, HEAP_NO_SERIALIZE, pDefault);

	return 1;
}