// Dynamic Array Design
// -> 일반적인 배열 처럼 한순간에 배열 크기만큼
//      물리 메모리가 할당되는 것이 아니라
//      사용량의 증가에 따라 물리 메모리에 할당되는
//      , 즉, 배열의 크기가 점진적으로 증가하는 배열

/*
1. 시스템의 pageSize, allocGranularity 를 얻어온다.

2. 메모리를 예약한다.

3. 필요한 만큼의 메모리를, 물리 메모리에 commit (할당)

4. 메모리 반환
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#define MAX_PAGE 10 // 할당할 최대 페이지 개수

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;
int PageFaultExceptionFilter(DWORD);

int _tmain(int argc, TCHAR* argv[])
{
	LPVOID baseAddr;
	int* lpPtr;
	SYSTEM_INFO sSysInfo;

	GetSystemInfo(&sSysInfo);
	pageSize = sSysInfo.dwPageSize;

	// MAX_PAGE 개수만큼 Reserve
	baseAddr = VirtualAlloc(
		// - 예약 및 할당하려는 메모리 시작 주소
		// - 일반적으로 NULL 전달
		//   NULL 이 할당되면, 할당하고자 하는 크기에 맞춰서
		//   메모리 위치를 임의로 결정한다.
		// - 하지만, Reserve 상태의 페이지를 Commit 상태로 변경할 때에는
		//   해당 페이지의 시작 주소를 지정해야 한다.
		NULL,                          // 임의 주소 할당

		// - 할당하고자 하는 크기 단위를 바이트 단위로 지정
		MAX_PAGE * pageSize, // 예약 메모리 크기

		// - 메모리 할당 타입
		MEM_RESERVE,            // RESERVE

		// - 페이지별 접근방식에 제한을 두는 용도
		// - Reserve 상태에 둘 때에는 접근을 허용하지 않는 PAGE_NOACCESS
		// - Commit 상태로 변경할 때에는,  읽기 쓰기 모두 허용하는 PAGE_READWRITE
		PAGE_NOACCESS         // NO ACCESS
	);

	if (baseAddr == NULL)
		_tprintf(_T("Virtual Alloc reserve failed"));

	// 배열의 시작 번지와 같은 주소 부여
	lpPtr = (int*)baseAddr;
	nextPageAddr = (int*)baseAddr;

	// page fault 발생 시 예외 발생
	for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	{
		__try
		{
			lpPtr[i] = i;
		}
		__except (PageFaultExceptionFilter(GetExceptionCode()))
		{
			// 예외처리 문제 발생시 종료
			ExitProcess(GetLastError());
		}
	}

	//for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	//{
	//	_tprintf(_T("%d \n"), lpPtr[i]);
	//}

	BOOL isSuccess = VirtualFree(
		baseAddr,         // 해제할 메모리의 시작 번지
		0,					   // 해제할 메모리 크기를 바이트 단위로 지정 


		// - MEM_RELEASE 지정할 경우, FREE 상태가 된다.
		//   MEM_RELEASE 전달시 2번째 인자는 0이 되어야 한다.
		//   MEM_RELEASE 전달시 lpAddr 의 값은, VirtualAlloc 함수 호출을 통해 예약된 메모리 시작 번지여야만 한다.
		// - MEM_DECOMMIT 전달할 경우, RESERVE 상태로
		//   2개 모두, 물리적 메모리가 할당되어 있다면, 해당 메모리 반환다.
		MEM_RELEASE // MEM_DECOMMIT 혹은 MEM_RELESE 중 하나 지정
	);

	if (isSuccess)
		_tprintf(_T("Release succeeded"));
	else
		_tprintf(_T("Release failed"));

	return 0;
};

int PageFaultExceptionFilter(DWORD exptCode)
{
	// 예외 원인이 page fault 가 아니라면
	if (exptCode != EXCEPTION_ACCESS_VIOLATION)
	{
		_tprintf(_T("Exception Code = %d \n"), exptCode);

		// __except 함수 실행
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// 물리 메모리에 할당이 이루어지지 않아서
	// 발생한 예외이므로, 문제가 된 페이지를 COMMIT 상태로 
	_tprintf(_T("Exception is a page fault \n"));

	if (pageCnt >= MAX_PAGE)
	{
		_tprintf(_T("Exception : Out Of Page \n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}

	LPVOID lpvResult = VirtualAlloc(
		(LPVOID)nextPageAddr,
		pageSize,
		MEM_COMMIT,
		PAGE_READWRITE
	);

	if (lpvResult == NULL)
	{
		_tprintf(_T("Virtual Alloc Failed \n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		_tprintf(_T("Allocating another page \n"));
	}

	pageCnt++;

	// COMMIT 상태에 있는 페이지의 위치 정보 갱신
	nextPageAddr += pageSize / sizeof(int);

	// page fault가 발생한 지점부터 실행 계속
	// (예외가 발생한 위치로 돌아가서 실행 이어갈 것이다.)
	return EXCEPTION_CONTINUE_EXECUTION;
}