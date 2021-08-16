#include<iostream>

using namespace std;

enum class EJob
{
	None,
	Knight,
	Archer,
	Magicion
};

struct Player
{
	char Name[32];
	EJob Job;
	int Attack;
	int Armor;
};

/*
Binary 파일 작업하기
*/

int main()
{
	FILE* FileStream;
	Player player;

	strcpy_s(player.Name, "철수");
	player.Job = EJob::Knight;
	player.Attack = 10;
	player.Armor = 5;

	// 확장자도 마음대로 할 수 있다.
	fopen_s(&FileStream, "Test1.lol","wb");

	if (FileStream)
	{
		int Number = 100;
		int Array[10];
		for (int i = 0; i < 10; i++)
		{
			Array[i] = i + 1;
		}

		// 파일에 원하는 데이터 쓰기
		// 1번인자 : 저장하고자 하는 변수의 주소가 들어간다
		// 2번인자 : 저장하고자 하는 타입의 크기를 지정한다
		// ex) sizeof(int)
		// 3번인자 : 저장하고자 하는 개수를 지정한다
		// - 2번인자*3번인자의 바이트 수만큼을
		// - 1번인자에 지정한 주소로부터 저장하게 된다.
		// 4번인자 : 파일스트림
		fwrite(&Number,sizeof(int),1,FileStream);
		fwrite(Array,sizeof(int),10,FileStream);
		// 구조체도 저장가능
		fwrite(&player, sizeof(Player), 1, FileStream);

		// --------------------------------------------

		fclose(FileStream);
	}

	Player player1 = {};
	int Number1 = 0;
	int Array1[10] = {};
	
	fopen_s(&FileStream, "Test1.lol", "rb");

	if (FileStream)
	{
		// fread를 할때도
		fread(&Number1,sizeof(int),1,FileStream);
		fread(&Array1, sizeof(int), 10, FileStream);
		fread(&player1, sizeof(Player), 1, FileStream);

		cout << "Number 1 : " << Number1 << endl;

		for (int i = 0; i < 10; i++)
		{
			cout << Array1[i] << endl;
		}

		switch (player1.Job)
		{
		case EJob::Knight:
			cout << "기사" << endl;
			break;
		case EJob::Archer :
			cout << "궁수" << endl;
			break;
		case EJob::Magicion :
			cout << "마법사" << endl;
			break;
		}

		fclose(FileStream);
	}

	return 0;
}