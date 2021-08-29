#pragma once
#include<string>

class CHash
{
private :
	unsigned __int64 m_HashKey;
public:
	template<typename Key>
	unsigned __int64 GetHash(const Key key)
	{
		m_HashKey = 0;
		size_t size = sizeof(key);
		unsigned __int64 NewKey = (unsigned __int64)key;
		for (size_t i = 0; i < size; i++)
		{
			unsigned char data = key & 0xff;
			NewKey += data;
			NewKey >>= 8;
		}
		m_HashKey = NewKey;
		return m_HashKey;
	}
	template<>
	unsigned __int64 GetHash(const char* &key)
	{
		m_HashKey = 0;
		size_t size = strlen(key);
		for (size_t i = 0; i < size; i++)
		{
			if (i % 2 == 0) m_HashKey += (unsigned __int64)key[i];
			else m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}
	template<>
	unsigned __int64 GetHash(const std::string key)
	{
		m_HashKey = 0;
		size_t size = key.length();
		for (size_t i = 0; i < size; i++)
		{
			if (i % 2 == 0) m_HashKey += (unsigned __int64)key[i];
			else m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}
};