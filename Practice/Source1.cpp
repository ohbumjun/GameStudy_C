#include<iostream>

using namespace std;

int main()
{
	FILE* FileStream;
	fopen_s(&FileStream, "Test1.txt", "wt");
	if (FileStream)
	{
		fputc('a', FileStream);
		fputs("�ȳ�\n", FileStream);
		fprintf(FileStream, "hello");
		int Number = 10;
		fprintf(FileStream, "%d �ȳ�", Number);
		fclose(FileStream);
	}
	fopen_s(&FileStream, "Test1.txt", "rt");
	if (FileStream)
	{
		fseek(FileStream, 0, SEEK_END);
		int FileSize = ftell(FileStream);
		fseek(FileStream, 0, SEEK_SET);
		cout << "FileSize : " << FileSize << endl;
	
		char c = fgetc(FileStream);
		cout << c << endl;
		char Line[128] = {};
		fgets(Line, 127, FileStream);
		cout << Line << endl;
		fclose(FileStream);
	}

	return 0;
}