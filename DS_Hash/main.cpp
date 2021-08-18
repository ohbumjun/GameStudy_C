#include<iostream>

/*
�ؽ���, ----------------
Ž�� �ӵ��� ���� ����
�˰���

HashTable
- ���� ����Ǵ� �����ͺ���,
�ξ� ū �޸� ������ ����� ����
�����͸� Hash �Լ��� ����
�ε����� ���Ѵ���
�����ϴ� ����̴�.

���� �����Ͱ� ����� ��, 
�������� ����Ǵ� ���� �ƴ϶�,

�迭�� ��������� �ΰ�
������ �Ǵ� ����̴�.

��, �޸𸮰� ����Ǵ� ����
���� �� ������, �޸𸮸�
����ؼ�, 
Ž���ӵ��� �ش�ȭ��Ű�� �˰����̴�.

Hash �浹 --------------
- �ߺ��� idx�� �����͸�
�����Ϸ��� �ϴ� ��� 

Hash���� �߿��� ����
1) ������ �˳�����
2) ���� Hash�Լ���
����ϰ� �ִ°����� 

*/

#include"Hash.h"
#include"HashTable.h"

using namespace std;

int main()
{
	CHashTable<const char*, const char*>	Table;


	Table.insert("�߽���", "����");
	Table.insert("���", "����");
	Table.insert("Ƽ��", "���۳�");
	Table.insert("Ʈ���ٹ̾�", "�̱�������");
	Table.insert("����", "�����ƴ³�");
	Table.insert("ABC", "ABC");
	Table.insert("ACB", "ACB");

	cout << "hello" << endl;

	std::cout << Table["�߽���"] << std::endl;
	std::cout << Table["Ʈ���ٹ̾�"] << std::endl;

	Table["Test"] = "Test��.";
	Table["Ʈ���ٹ̾�"] = "������ ������";
	std::cout << Table["Test"] << std::endl;
	std::cout << Table["Ʈ���ٹ̾�"] << std::endl;
	std::cout << Table["Ƽ��"] << std::endl;


	CHashTable<const char*, const char*>::iterator	iter = Table.Find("Ƽ��");

	if (Table.IsValid(iter))
		std::cout << "Ƽ��� �������� Ű���Դϴ�." << std::endl;

	iter = Table.Find("��ũ��");

	if (Table.IsValid(iter))
		std::cout << "��ũ���� �������� Ű���Դϴ�." << std::endl;

	else
		std::cout << "��ũ���� �߸��� Ű���Դϴ�." << std::endl;

	return 0;
}