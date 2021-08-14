#include<iostream>
#include<fstream>

int main()
{
	std::ofstream write;
	write.open("Stream.txt");

	// ������ �����ٸ�
	if (write.is_open())
	{
		char Text[128] = {};
		strcpy_s(Text, "������������");
		// null ���� ���� ������ �ȴ�
		write.write(Text,strlen(Text));
	}
	write.close();


	// ifstream : ���� ���� �� ���
	std::ifstream read;
	read.open("Stream.txt");
	if (read.is_open())
	{
		// ���ϳ����� �о����
		while (!read.eof())
		{
			// read.eof(): ���ϳ��� ���� true
			char Text[128] = {};
			// ���� �о����
			read.getline(Text, 128);
			std::cout << Text << std::endl;
		}
	}
	read.close();

	return 0;
}