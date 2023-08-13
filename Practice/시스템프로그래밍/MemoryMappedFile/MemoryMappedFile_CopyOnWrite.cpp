#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 파일에 데이터를 저장하고
// 저장된 데이터를 정렬하기

void SortIntData(int* pSortArr, int num);

int _tmain(int argc, TCHAR* argv[])
{
	// 1단계 파일 생성
	// - 파일 핸들을 얻어야만 한다.
	HANDLE hFile =
		CreateFile(
			_T("data.dat"),
			// 익기, 쓰기 모두 가능하도록 접근 권한 지정
			// COW 를 위해서는 파일을 읽기,쓰기 모드로 개방되어야 하자
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			// CREATE_ALWAYS,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	if (hFile == INVALID_HANDLE_VALUE)
		_tprintf(_T("Could not open file\n"));

	// 2단계 : 파일 연결 오브젝트 생성
	// - 메모리에 연결할 파일 정보를 담고 있는
	//   커널 오브젝트를 생성하는 것이다.
	HANDLE hMapFile =
		CreateFileMapping(
			hFile,
			// 커널 오브젝트 생성하는 다른 함수 같이 보안 속성 지정
			NULL,
			// 파일과 연결된 메모리의 접근 권한을 지정한다.
			// 아래 변수를 통해 COW 기반으로 동작하게 한다.
			PAGE_WRITECOPY,
			// 파일 연결 최대 메모리 크기
			// 상위 4바이트
			// 대용량 파일이 아니라면 0으로 충분
			0,
			// 파일 연결 최대 메모리 크기
			// 하위 4바이트
			// 0 을 전달하면, 첫번째 인자로 전달된
			// 핸들의 파일 크기로 지정된다.
			0,
			// 파일 연결 오브젝트의 이름
			NULL
		);

	if (hMapFile == NULL)
		_tprintf(_T("Could not create map of file\n"));

	// 3단계 : 메모리에 연결
	// - 실제로 가상 메모리에 파일을 연결한다.
	int* pWrite =
		(int*)MapViewOfFile(
			hMapFile,
			// 연결된 메모리의 접근 권한
			// CreateFileMapping 함수의 3번째 인자 설정이
			// PAGE_READWRITE 라면
			// 읽기와 쓰기 중 하나 혹은 둘다 
			// PAGE_READ 라면 오로지 읽기 권한
			// COW 를 위해 아래와 같이 세팅한다.
			FILE_MAP_COPY,
			// 지정한 파일 전부를 메모리에 연결해야 하는 것은
			// 아니다. 파일의 일부 영역만 선택해서 연결하는 것도
			// 가능하다
			// 메모리에 연결할 파일의 오프셋(시작번지) 지정
			// 상위 4바이트 지정
			0,
			// 하위 4바이트
			0,
			// 메모리에 연결할 실제 크기를 바이트 단위로 지정
			// 오프셋 기준으로 지정된 바이트 만큼 메모리에 연결
			// 0을 전달하면, 지정된 오프셋에서부터
			// 파일 끝까지 메모리에 연결
			0
		);

	if (pWrite == NULL)
		_tprintf(_T("Could not map view of file\n"));

	// 해당 파일에 데이터를 아래와 같이 덮어쓰고 있음을
	// 알아볼 수 있다.
	// 하지만, 실제 파일에는 반영되지 않는다.
	// COW 기반으로 동작했기 때문이다.
	// 즉, MMF 기반에서 데이터를 변경하려는 순간에
	// 파일의 복사본을 만들어서 작업했기 때문에
	// 원본은 그대로 보존된 것이다.
	pWrite[0] = 1;
	pWrite[1] = 3;
	pWrite[2] = 0;
	pWrite[3] = 2;
	pWrite[4] = 4;
	pWrite[5] = 5;
	pWrite[6] = 8;
	pWrite[7] = 6;
	pWrite[8] = 7;

	// SortIntData(pWrite, 9);

	_tprintf(_T("%d, %d, %d\n"), pWrite[0], pWrite[1], pWrite[2]);
	_tprintf(_T("%d, %d, %d\n"), pWrite[3], pWrite[4], pWrite[5]);
	_tprintf(_T("%d, %d, %d\n"), pWrite[6], pWrite[7], pWrite[8]);

	// 해당 함수 호출을 통해
	// 할당되었던 가상 메모리와의 연결을 끊는다.
	// 이것을 가장 먼저 해줘야 한다.
	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return 0;
};

void SortIntData(int* pSortArr, int num)
{
	// bubble sort
	int temp;

	for (int i = 0; i < num - 1; ++i)
	{
		for (int j = 1; j < num; ++j)
		{
			if (pSortArr[j - 1] > pSortArr[j])
			{
				temp = pSortArr[j - 1];
				pSortArr[j - 1] = pSortArr[j];
				pSortArr[j] = temp;
			}
		}
	}
}