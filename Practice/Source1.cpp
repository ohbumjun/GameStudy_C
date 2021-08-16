#include<iostream>

using namespace std;

enum class EJob
{
	Knight
};

struct Player
{
	char Name[32];
	int Attack;
	EJob Job;
};

int main()
{
	FILE* FileStream;
	fopen_s(&FileStream, "test1.lol", "wb");
	if (FileStream)
	{
		int Attack = 1;
		int Array[10] = {};
		for (int i = 0; i < 10; i++)
			Array[i] = i;
		Player player;
		strcpy_s(player.Name, "Ã¶¼ö");
		player.Attack = 10;
		
		fwrite(&Attack,sizeof(int),1,FileStream);
		fwrite(Array,sizeof(int),10,FileStream);
		fwrite(&player,sizeof(Player),1,FileStream);
	
		fclose(FileStream);
	}

	fopen_s(&FileStream, "test1.lol", "rb");
	if (FileStream)
	{
		int Attack;
		int Array[10];
		Player player1 = {};

		fread(&Attack, sizeof(int), 1, FileStream);
		fread(Array, sizeof(int), 10, FileStream);
		fread(&player1, sizeof(Player), 1, FileStream);
		cout << Attack << endl;
		cout << Array[0] << endl;
		cout << player1.Name << endl;
	}

	return 0;
}