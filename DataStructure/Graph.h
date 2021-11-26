/*
���ڼ��� ���� 3����
- Single Byte Charadter Set
���� ǥ���ϴµ� 1����Ʈ ��� = �ƽ�Ű �ڵ�
- Multi Byte Character Set
�ѱ��� 2����Ʈ, ������ 1 ����Ʈ
- Wide Byte Character SEt
���ڸ� ǥ���ϴµ� 2����Ʈ ���
�����ڵ� 
*/


// �����ڵ� ���
// 1. char�� ����ϴ� wchar_t
// 2. "ABC"�� ����ϴ� L"ABC"
// �׳� "ABC" --> �ƽ�ũ �ڵ�� ǥ���ϰڴٴ� �Ϲ��� �ǹ�

// ex
wchar_str str[] = L"ABC";

int main()
{
	wchar_t str[] = L"ABC";

	// ���ڿ��� �ٷ�� ����� �������
	// �Ҵ�� �޸�ũ��� �״��
	// ��, ����� 1 ����Ʈ, �ѱ��� 2����Ʈ �� �޸� ũ�⸦ �Ҵ��Ѵٴ� ���̴� 
	int size = sizeof(str);

	// ���ڿ� ���̸� ��ȯ�ϴ� �Լ�
	// strlen(char*) : char* Ÿ���� ���ڷ� ����Ѵ�.
	//  wcslen(wchar_t* ) : wchar_t* Ÿ���� ���ڷ� ����Ѵ�.

	// ���ڿ� ���� 3���� �ν� 
	// �ѱ� �ϳ��� 1���� ���ڷ� �ν��Ѵ�. �����ڵ�� 
	int len = wcslen(str);

	 // �Ʒ��� �ƽ�Ű �ڵ� ��� ó��
	printf("�迭 ũ�� : %d", size);

	// ���� ���ؼ� ���� �ڵ� ��� �Լ��� ����ϱ�
	wprintf(L"Array Size : ", size);

	return 0;
}

