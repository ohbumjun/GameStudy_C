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
	//  64bit 정수 -- 한 정수당 8byte 차지 
	unsigned __int64 m_HashKey;
public :
	template<typename Key>
	unsigned __int64 GetHash(Key key)
	{
		m_HashKey = 0;
		size_t Length = sizeof(key);

		unsigned __int64 NewKey = (unsigned __int64)key;

		// 1 바이트 단위로 접근할 것이다 
		for (size_t i = 0; i < Length; i++)
		{
			// 둘다 1인 애들만 저장될 것이다. 
			unsigned char data = NewKey & 0xff;
			m_HashKey += data;
			NewKey >>= 8;
		}
	}
};