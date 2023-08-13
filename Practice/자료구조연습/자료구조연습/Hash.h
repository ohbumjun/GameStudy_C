#pragma once

#include <string>

class CHash {
public :
	CHash(){}
	~CHash(){}
private :
	unsigned __int64 m_HashKey;
public :
	template<typename Key>
	unsigned __int64 GetKey(const Key& key)
{
		m_HashKey = 0;

		unsigned __int64 NewKey = (unsigned __int64)key;

	for (size_t i = 0; i < sizeof(NewKey); ++i)
	{
		m_HashKey += (NewKey & 0xff);
		NewKey >>= 8;
	}

	return m_HashKey;
}
	template<>
	unsigned __int64 GetKey(const std::string& key)
{
		m_HashKey = 0;

		size_t KeyLen = key.length();

	for (size_t i = 0; i < KeyLen; ++i)
	{
		unsigned __int64 data = key[i] & 0xff;

		if (i % 2 == 0)
			m_HashKey += data;
		else
			m_HashKey *= data;
	}
	return m_HashKey;
}

	template<>
	unsigned __int64 GetKey(char* const & key)
{
		size_t KeyLen = sizeof(key);
		m_HashKey = 0;

	for(size_t i = 0; i < KeyLen; ++i)
	{
		unsigned __int64 data = key[i] & 0xff;

		if (i % 2 == 0)
			m_HashKey += data;
		else
			m_HashKey *= data;
	}

	return m_HashKey;
}
};