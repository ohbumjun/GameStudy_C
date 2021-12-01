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
private  :
	//  64bit ���� -- �� ������ 8byte ���� 
	unsigned __int64 m_HashKey;
public :
	template<typename Key>
	unsigned __int64 GetHash(Key key)
	{
		m_HashKey = 0;
		size_t Length = sizeof(key);

		unsigned __int64 NewKey = (unsigned __int64)key;

		// 1 ����Ʈ ������ ������ ���̴� 
		for (size_t i = 0; i < Length; i++)
		{
			// �Ѵ� 1�� �ֵ鸸 ����� ���̴�. 
			unsigned char data = NewKey & 0xff;
			m_HashKey += data;
			NewKey >>= 8;
		}
	}
};