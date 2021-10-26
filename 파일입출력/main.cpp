#include<iostream>

using namespace std;

/*
���� ����� :

���� ���� 1) txt  2) binary ���� 

���� ������̶�, 
�ϵ� ��ũ�� ������ ����ų� , �ִ� ������
�����͸� ���� �� ����Ѵ�.

C��� ��� : fopen �Լ��� �̿��ؼ� ó���Ѵ�
C++   ��� : ifstream, ofstream�� �̿��ؼ� ó���Ѵ�.

-----------

����� :
���� ���� ������Ʈ ������ �ִ� ���� �������� ������.
������, Debug ���� ���� .exe ������ ������ �Ǳ⵵ �Ѵ�

*/

// fopen_s() �� ���� �����ͷ� ���ڸ� �޴´�. �� ���� ?
// �Ʒ��� ���� ��찡 �ִ�.
class CPlayer
{};
// ���� �����͸� ���ڷ� �ѱ��.
void CreatePlayer(CPlayer** ppPlayer)
{
	// �������� ����
	// ���� ������ ������ �����Ҵ���
	// CPlayer ��ü�� �ּҸ� �ش�.
	*ppPlayer = new CPlayer;
}

int main()
{
	CPlayer* pPlayer = nullptr;
	CreatePlayer(&pPlayer);
	delete pPlayer;
	
	FILE* FileStream = nullptr;

	// 1) ���������� �Ѱ��ش�
	// 2) ���ϴ� ���ϱ����� ���
	// 3) Mode 
	// - r : ������ �о�´�
	// - w : ������ �����
	// - a : ���Ͽ� �����Ͽ� �̾�⸦ �Ѵ�.
	// - r+ : ������ �����ϸ� �ش� ������ �а�, 
	// ���� �����ϰ� ����
	// ������ ���ٸ� ���� ��ȯ�Ѵ�.
	// - w+ : ������ �����ϸ�
	// �ش� ������ �а� ���� �����ϰ� ����
	// ������ ����� ���� ������ش�
	// - a+ : ������ �����ϸ�, �ش� ������ �� ������
	// �а� ���Ⱑ �Ѵ� �����ϰ� ����.
	// ������ ���ٸ�, ���� �����.

	// t : �ؽ�Ʈ ����
	// b : ���̳ʸ� ����
	// rt : �ؽ�Ʈ ���� �Ͼ�´�
	// rb : ���̳ʸ� ������ �о�´�
	// wt : �ؽ�Ʈ Ÿ���� �����
	// wb : ���̳ʸ� ������ ���ܴ�
	// at, ab
	fopen_s(&FileStream,"Test.txt","wt"); 

	if (FileStream != nullptr)
	{
		// fputc : ���Ͽ� ���� �ϳ��� �߰��Ѵ�.
		fputc('A',FileStream);
		fputc('B', FileStream);

		// fputs : ���� �������� �߰��Ѵ�.
		fputs("���� �������Դϴ�.\n",FileStream);
		fputs("�ι�° ���Դϴ�.\n", FileStream);

		// fprintf : ���ڵ� ���� �� �ִ�!! 
		fprintf(FileStream, "fprintf ����Դϴ�\n");

		int Number = 100;

		// %d ����, %s ���ڿ�, %f : �Ǽ�
		// %d ��ſ�, ���� ���ڿ��� ������ִ� ���̴�
		fprintf(FileStream, "���ڴ� %d�Դϴ�\n", Number);

		// ���Ͻ�Ʈ���� �����ٸ�
		// ������ �ݾ���� �Ѵ�.
		fclose(FileStream);
	}

	// �о���� �۾�
	fopen_s(&FileStream, "Test.txt", "rt");
	if (FileStream)
	{
		// ���� �ϳ� �о����
		char c = fgetc(FileStream);

		std::cout << c << std::endl;

		// ���� ������ �о����
		char Line[128] = {};

		// �ִ� 128������(�������� null ����)
		// ��, ���ٸ� �о���� �ȴ�.
		// ��, fgets �Լ��� \n�� ������ �Ǹ�, �ű������ �о���� �ȴ�.
		// �� \n���� �о�ͼ�, ���� cout <<  �ÿ���, ������ �ѹ� �� ���¿��� ����Ǵ� ���̴�.
		fgets(Line, 127, FileStream);
		cout << Line << endl;
		fgets(Line, 127, FileStream);
		cout << Line << endl;

		// ������ ũ�� ���غ���
		// fseek �Լ� 
		// 1) ���� ��Ʈ��
		// 2) 3�� ���ڿ� ������ ���������κ���
		// �����Ʈ�� �̵��� �������� �����Ѵ�.
		// 3) origin --> ��� ��������(������)\
		// - SEEK_CUR : ���� ������������ ��ġ.
		// ex) ���� ���� ó�� ������ ����, ���� ó��
		// Ŀ���� ��ġ���ִ� ��
		// - SEEK_END : ������ ���� �� ��ġ�� �̵�
		// - SEEK_SET : ������ ������ġ�� �̵� 
		fseek(FileStream, 0, SEEK_END);

		// ftell : ���� ���� Ŀ���� ��ġ�� ���´�
		int FileSize = ftell(FileStream);

		// ����Ŀ���� �� ������ �̵���Ű��
		// ����Ŀ�� ��ġ�� ����ϸ�
		// ���� ũ�Ⱑ ������ �ȴ�
		// ( byte ���� )�� �Դٰ��� �Ѵ�.
		cout << "FileSize : " << FileSize << endl;

		// �ٽ� ó�� ���� ��ġ��
		fseek(FileStream, 0, SEEK_SET);

		// �о����� ������ �ݾ���� �Ѵ�.
		fclose(FileStream);
	}
	return 0;
}
