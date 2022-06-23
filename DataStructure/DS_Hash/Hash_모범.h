#pragma once

/*
HashTable :
���� ����Ǵ� �����ͺ���, �ξ� ū �޸� ������ 
�������� �����͸� Hash�Լ��� ����
�ε����� ���Ѵ��� �����ϴ� ����̴�.

���� �����Ͱ� ����� ��, �������� ����Ǵ�
���� �ƴ϶�, �迭�� ��������� �ΰ�
������ �Ǵ� ����̴�.

��, �޸� ����Ǵ� ���� ���� �� ������
�޸𸮸� ����ؼ� 
Ž�� �ӵ��� �ش�ȭ ��Ű�� �˰������̴�. 

*/

#include<string>

class CHash
{
public :
	CHash(){}
	~CHash(){}
private :
	unsigned __int64 m_HashKey;
public :
	template<typename Key>
	unsigned __int64 GetHash(Key key)
	{
		m_HashKey = 0;
		// key Ÿ���� ũ�� 
		size_t Length = sizeof(key);
		unsigned __int64 NewKey = (unsigned __int64)key;
		
		// byte ����ŭ �ݺ��� ���̴�
		// 1byte�� �̾ƿ� ���̴� 
		// for���� byte ������ ���� �ȴ�. 
		// int�� 4byte�̴�.
		// for���� 1byte�� 4�� ���� �Ǵ� ���̴�
		// �� ������ 1byte �����ͼ�, ���
		// 8bit ( 1byte ) ������ �а�
		// �� �� ������ ���.
		// ���� ���� 1byte���� �������� ���̴�
		// ���� unsigned char , ��, 1byte�� �������� ���̴�
		// 
		for (size_t i = 0; i < Length; i++)
		{
			// ff : 8byte --> 16���� �ϳ��� 2���� 4��ǥ�� --> 4bit ǥ�� 
			unsigned char data = NewKey & 0xff;
			m_HashKey += data;
			NewKey >>= 8;
		}
		return m_HashKey;
	}

	// ���ø� Ư��ȭ
	// ���ø��� ���� Ÿ�Կ� ���� 
	// ������ ó���Լ��� �������� �۾��� �� �ְ� ���ش�
	template<>
	unsigned __int64 GetHash(std::string key)
	{
		m_HashKey = 0;
		size_t Length = key.length();
		for (size_t i = 0; i < Length; i++)
		{
			// ¦�� ��° �׸��� �����ְ�
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			// Ȧ�� ��° �׸��� �����ְ� 
			else
				m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}

	template<>
	unsigned __int64 GetHash(const char* key) //
	{
		m_HashKey = 0;
		// size_t 
		// 32 bit : default
		// __int64 : 64bit int�� ������. 
		// ��, 32bit ���ߴܰ迡���� 32 bit 
		// 64bit ���ߴܰ迡���� 64bit 
		// ���� ���� unsigned int�� typedef ����
		// �޸𸮳� , ���ڿ��� ���̸� ���� ������ 
		// size_t ��� ������ ���̰� ��ȯ�ȴ�. 
		size_t Length = strlen(key);
		for (size_t i = 0; i < Length; i++)
		{
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			else
				m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}

};