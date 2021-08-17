#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	std::ofstream write;
	write.open("hello.txt");
	if (write.is_open())
	{
		char Text[128] = {};
		strcpy_s(Text, "머마마마");
		write.write(Text, strlen(Text));
	}
	write.close();

	std::ifstream read;
	read.open("hello.txt");
	if (read.is_open())
	{
		while (!read.eof())
		{
			char Text[128] = {};
			read.getline(Text, 127);
			cout << Text << endl; 
		}
	}

	return 0;
}