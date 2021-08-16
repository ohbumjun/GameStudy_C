#include<iostream>

int main()
{
	FILE* FileStream;
	fopen_s(&FileStream, "Test.txt", "wt");
	if (FileStream != nullptr)
	{
		fputc('a', FileStream);
		fputs("aaa\n", FileStream);
		fprintf(FileStream, "Hello");
		int Number = 10;
		fprintf(FileStream, "%d¿‘¥œ¥Ÿ", Number);
	}

	return 0;
}