#pragma once
#include<string>

template<typename KEY, typename VALUE>
class CHash
{
private :
	unsigned __int64 m_HashKey;
public :
	template<typename KEY, typename VALUE>
	unsigned __int64 GetHashKey(KEY key)
	{
		m_HashKey = 0;
		size_t Length = sizeof(key);
		unsigned __int64 HashKey = unsigned __int64(key);
		for (size_t i = 0; i < Length; i++)
		{
			unsigned char Number = HashKey && 0xff;
			m_HashKey += Number;
			HashKey >>= 8;
		}
		return m_HashKey;

	};
	template<>
	unsigned __int64 GetHashKey(std::string key)
	{
		m_HashKey = 0;
		size_t Length = key.length();
		for (size_t i = 0; i < Length; i++)
		{
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			else
				m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}
	template<>
	unsigned __int64 GetHashKey(const char* key)
	{
		m_HashKey = 0;
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