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
Binary ���� �۾��ϱ�
*/

int main()
{
	FILE* FileStream;
	Player player;

	strcpy_s(player.Name, "ö��");
	player.Job = EJob::Knight;
	player.Attack = 10;
	player.Armor = 5;

	// Ȯ���ڵ� ������� �� �� �ִ�.
	fopen_s(&FileStream, "Test1.lol","wb");

	if (FileStream)
	{
		int Number = 100;
		int Array[10];
		for (int i = 0; i < 10; i++)
		{
			Array[i] = i + 1;
		}

		// ���Ͽ� ���ϴ� ������ ����
		// 1������ : �����ϰ��� �ϴ� ������ �ּҰ� ����
		// 2������ : �����ϰ��� �ϴ� Ÿ���� ũ�⸦ �����Ѵ�
		// ex) sizeof(int)
		// 3������ : �����ϰ��� �ϴ� ������ �����Ѵ�
		// - 2������*3�������� ����Ʈ ����ŭ��
		// - 1�����ڿ� ������ �ּҷκ��� �����ϰ� �ȴ�.
		// 4������ : ���Ͻ�Ʈ��
		fwrite(&Number,sizeof(int),1,FileStream);
		fwrite(Array,sizeof(int),10,FileStream);
		// ����ü�� ���尡��
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
		// fread�� �Ҷ���
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
			cout << "���" << endl;
			break;
		case EJob::Archer :
			cout << "�ü�" << endl;
			break;
		case EJob::Magicion :
			cout << "������" << endl;
			break;
		}

		fclose(FileStream);
	}

	return 0;
}