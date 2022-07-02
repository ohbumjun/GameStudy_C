// CreateProcess : 덧셈 프로세스를 생성
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define DIR_LEN MAX_PATH + 1

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si = { 0, };

	// 해당 구조체 변수는, 새로 생성되는 프로세스 관련 정보를 얻기 위해 사용된다.
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);
	// STARTF_USEPOSITION -> 구조체 멤버 중에서 dwX, dwY 반영
	// STARTF_USESIZE -> 구조체 멤버 중에서 dwXSize, dwYSize 반영
	si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	si.dwX = 100; // 프로세스 윈도우 x 좌표
	si.dwY = 200; // 프로세스 윈도우 y 좌표
	si.dwXSize = 300;  // 프로세스 윈도우 x 크기
	si.dwYSize = 200;  // 프로세스 윈도우 y 크기
	// si.lpTitle = _T("I am a boy"); -> 콘솔 윈도우의 타이틀 바 제목

	// 현재 만들어낼 프로세스 main 함수에 전달할 인자이다.
	// - AdderProcess.exe 는 실행 파일 이름이고
	// - 10, 20 이라는 2개의 숫자가 바로, main 함수의 전달인자가 된다.
	TCHAR command[] = _T("AdderProcess.exe 10 20");
	TCHAR cDir[DIR_LEN];
	BOOL state;

	// 현재 디렉토리 확인
	GetCurrentDirectory(DIR_LEN, cDir);
	_fputts(cDir, stdout);
	_fputts(_T("\n"), stdout);

	// CreateProcess.exe 는 어디에 존재하든 상관없다.
	// 프로그램이 실행되면서, 현재 디렉터리를 아래의 경로로 변환하기 때문이다.
	SetCurrentDirectory(_T("C:\\WinSystem"));

	// 현재 디렉토리 확인
	// - 일반적으로 프로세스가 생성되면, 프로세스의 현재 디렉토리는
	// 프로세스의 실행파일이 존재하는 디렉터리로 설정된다.
	// 아래 함수를 통해서, 실행 중에 있는 프로세스의 현재 디렉토리 위치 확인할 수 있다.
	GetCurrentDirectory(DIR_LEN, cDir);
	_fputts(cDir, stdout);
	_fputts(_T("\n"), stdout);

	// 프로세스 생성
	state = CreateProcess(
		NULL, // 실행 파일 이름
		command, // main 함수에 전달될 문자열 -> 여기에서 실행 파일 이름이 전달될 
						// 경우, 표준 검색 경로 (찾아보기) 를 통해 실행 파일을 찾게 된다.
		NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE,
		NULL, NULL, &si, &pi
	);

	if (state != 0)
		_fputts(_T("Creation OK \n"), stdout);
	else
		_fputts(_T("Creation Error !\n"), stdout);

	return 0;
}