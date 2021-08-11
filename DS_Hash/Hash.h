#pragma once

#include<string>


class CHash
{
public :
	CHash()
	{

	}
	~CHash()
	{

	}
private :
	// unsigened : ��� only
	unsigned __int64 m_HashKey;

public :
	// ���ø� Ư��ȭ 
	template<typename KEY>
	unsigned __int64 GetHash(KEY key)
	{
		// �پ��� type�� key�� ����
		// Hash�Լ��� ���� ���̴� 
		
		// �ִ��� key�鳢��
		// �浹�� ���� ���� �ϴ� �˰��� ��Ʈ��
		// �ٷ� �� �κ��̴�.
		
		// ���۽� 0���� �����
		m_HashKey = 0;
		size_t = Length = sizeof(key);
		unsigned __int64 NewKey = (unsigned __int64)key;

		for (size_t i = 0; i < Length; i++)
		{
			// ���������� ��� �и鼭
			// 1byte, 8bit�� ������ ���̴�

			// �� ������ bit ��Ҹ� �������� �� ���̴�
			// 1111111 & �����ϰ� �Ǹ�
			// 
			unsigned char data = NewKey & 0xff;
		
			// 1byte�� ��� ���Ѵ�
			m_HashKey += data;
			// ���������� 8bit �δ�
			NewKey >>= 8;
		}
		return m_HashKey;
	}

		// ���ø� Ư��ȭ�� ?
		// ������ type�� ����
		// Ư��ȭ�� �Լ��� ȣ���� �� �ִ�
		// type�� ����
		// ������ ó���Լ��� ��������
		// �۾��� �� �ְ� ���ش�

		// string type�� ���ڷ� ������ �Ǹ�
		// �Ʒ� �Լ��� ����ȴ�.
		template<>
		unsigned __int64 GetHash(std::string key)
		{
			m_HashKey = 0;
			size_t Length = key.length();
			for (size_t i = 0; i < Length; i++)
			{
				if (i % 2 == 0)
					// �ƽ�Ű �ڵ尪 �����ֱ�
					m_HashKey += (unsigned __int64)key[i];
				else
					m_HashKey *= (unsigned __int64)key[i];
			}
			return m_HashKey;
		}

		template<>
		unsigned __int64 GetHash(const char* key)
		{
			m_HashKey = 0;
			
			// strlen : ���ڿ� ���� �����ִ� �Լ�
			// size_t : unsigned int : ������ 4byte
			// ���� --> __int64�� �س�����, 8byte
			size_t Length = strlen(key);
			
			for (size_t i = 0; i < Length; i++)
			{
				if (i % 2 == 0)
					// �ƽ�Ű �ڵ尪 �����ֱ�
					m_HashKey += (unsigned __int64)key[i];
				else
					m_HashKey *= (unsigned __int64)key[i];
			}
			return m_HashKey;
		}

};

