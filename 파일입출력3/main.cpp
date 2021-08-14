#include<iostream>
#include<fstream>

int main()
{
	std::ofstream write;
	write.open("Stream.txt");

	// 파일이 열렀다면
	if (write.is_open())
	{
		char Text[128] = {};
		strcpy_s(Text, "마마마마마마");
		// null 빼고 길이 넣으면 된다
		write.write(Text,strlen(Text));
	}
	write.close();


	// ifstream : 파일 읽을 때 사용
	std::ifstream read;
	read.open("Stream.txt");
	if (read.is_open())
	{
		// 제일끝까지 읽어오기
		while (!read.eof())
		{
			// read.eof(): 파일끝에 가면 true
			char Text[128] = {};
			// 한줄 읽어오기
			read.getline(Text, 128);
			std::cout << Text << std::endl;
		}
	}
	read.close();

	return 0;
}