#include<iostream>

using namespace std;

/*
파일 입출력 :

파일 종류 1) txt  2) binary 파일 

파일 입출력이란, 
하드 디스크에 파일을 만들거나 , 있는 파일의
데이터를 얻어올 때 사용한다.

C언어 방식 : fopen 함수를 이용해서 처리한다
C++   방식 : ifstream, ofstream을 이용해서 처리한다.

-----------

상대경로 :
보통 현재 프로젝트 파일이 있는 곳을 기준으로 잡힌다.
하지만, Debug 파일 내에 .exe 파일이 기준이 되기도 한다

*/

// fopen_s() 는 이중 포인터로 인자를 받는다. 그 이유 ?
// 아래와 같은 경우가 있다.
class CPlayer
{};
// 이중 포인터를 인자로 넘긴다.
void CreatePlayer(CPlayer** ppPlayer)
{
	// 역참조를 통해
	// 단일 포인터 변수에 동적할당한
	// CPlayer 객체의 주소를 준다.
	*ppPlayer = new CPlayer;
}

int main()
{
	CPlayer* pPlayer = nullptr;
	CreatePlayer(&pPlayer);
	delete pPlayer;
	
	FILE* FileStream = nullptr;

	// 1) 이중포인터 넘겨준다
	// 2) 원하는 파일까지의 경로
	// 3) Mode 
	// - r : 파일을 읽어온다
	// - w : 파일을 만든다
	// - a : 파일에 접근하여 이어쓰기를 한다.
	// - r+ : 파일이 존재하면 해당 파일을 읽고, 
	// 쓰기 가능하게 연다
	// 파일이 없다면 에러 반환한다.
	// - w+ : 파일이 존재하면
	// 해당 파일을 읽고 쓰기 가능하게 연다
	// 파일이 없드면 새로 만들어준다
	// - a+ : 파일이 존재하면, 해당 파일의 맨 끝부터
	// 읽고 쓰기가 둘다 가능하게 연다.
	// 파일이 없다면, 새로 만든다.

	// t : 텍스트 파일
	// b : 바이너리 파일
	// rt : 텍스트 파일 일어온다
	// rb : 바이너리 파일을 읽어온다
	// wt : 텍스트 타일을 만든다
	// wb : 바이너리 파일을 만단다
	// at, ab
	fopen_s(&FileStream,"Test.txt","wt"); 

	if (FileStream != nullptr)
	{
		// fputc : 파일에 문자 하나를 추가한다.
		fputc('A',FileStream);
		fputc('B', FileStream);

		// fputs : 문자 여러줄을 추가한다.
		fputs("문자 여러줄입니다.\n",FileStream);
		fputs("두번째 줄입니다.\n", FileStream);

		// fprintf : 숫자도 넣을 수 있다!! 
		fprintf(FileStream, "fprintf 출력입니다\n");

		int Number = 100;

		// %d 정수, %s 문자열, %f : 실수
		// %d 대신에, 값을 문자열로 만들어주는 것이다
		fprintf(FileStream, "숫자는 %d입니다\n", Number);

		// 파일스트링을 열엇다면
		// 무조건 닫아줘야 한다.
		fclose(FileStream);
	}

	// 읽어오는 작업
	fopen_s(&FileStream, "Test.txt", "rt");
	if (FileStream)
	{
		// 문자 하나 읽어오기
		char c = fgetc(FileStream);

		std::cout << c << std::endl;

		// 문자 여러개 읽어오기
		char Line[128] = {};

		// 최대 128개까지(마지막은 null 문자)
		// 단, 한줄만 읽어오게 된다.
		// 즉, fgets 함수는 \n을 만나게 되면, 거기까지만 읽어오게 된다.
		// 즉 \n까지 읽어와서, 차후 cout <<  시에는, 개행이 한번 된 상태에서 진행되는 것이다.
		fgets(Line, 127, FileStream);
		cout << Line << endl;
		fgets(Line, 127, FileStream);
		cout << Line << endl;

		// 파일의 크기 구해보기
		// fseek 함수 
		// 1) 파일 스트림
		// 2) 3번 인자에 지정된 기준점으로부터
		// 몇바이트를 이동할 것인지를 지정한다.
		// 3) origin --> 어디를 기준으로(기준점)\
		// - SEEK_CUR : 현재 파일포인터의 위치.
		// ex) 만약 제일 처음 파일을 열면, 제일 처음
		// 커서가 위치해있는 것
		// - SEEK_END : 파일의 가장 끝 위치로 이동
		// - SEEK_SET : 파일의 시작위치로 이동 
		fseek(FileStream, 0, SEEK_END);

		// ftell : 현재 파일 커서의 위치를 얻어온다
		int FileSize = ftell(FileStream);

		// 파일커서를 맨 끝으로 이동시키고
		// 파일커서 위치를 출력하면
		// 파일 크기가 나오게 된다
		// ( byte 단위 )로 왔다갔다 한다.
		cout << "FileSize : " << FileSize << endl;

		// 다시 처음 파일 위치로
		fseek(FileStream, 0, SEEK_SET);

		// 읽었으면 무조건 닫아줘야 한다.
		fclose(FileStream);
	}
	return 0;
}
