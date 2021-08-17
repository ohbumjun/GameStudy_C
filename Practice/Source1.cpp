#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	std::ofstream write;
	write.open("hello.txt");
	if (write.is_open())
	{
		char Line[128] = {};
		strcpy_s(Line, "原原\n");
		write.write(Line, strlen(Line));
		strcpy_s(Line, "原原");
		write.write(Line, strlen(Line));
	}

	std::ifstream read;
	read.open("hello.txt");
	if (read.is_open())
	{
		while (!read.eof())
		{
			char Line[128] = {};
			read.getline(Line, 128);
			cout << Line << endl;
		}
	}

	return 0;
}