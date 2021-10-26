#pragma once

/*
HashTable :
실제 저장되는 데이터보다, 훨씬 큰 메모리 공간을 
만들어놓고 데이터를 Hash함수에 의해
인덱스를 구한다음 저장하는 방식이다.

실제 데이터가 저장될 때, 연속으로 저장되는
것이 아니라, 배열에 빈공간들을 두고
저장이 되는 방식이다.

즉, 메모리 낭비되는 것을 막을 수 없지만
메모리를 희생해서 
탐색 속도를 극대화 시키는 알고리즘이다. 

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
		// key 타입의 크기 
		size_t Length = sizeof(key);
		unsigned __int64 NewKey = (unsigned __int64)key;
		
		// byte 수만큼 반복할 것이다
		// 1byte를 뽑아올 것이다 
		// for문은 byte 단위로 돌게 된다. 
		// int는 4byte이다.
		// for문은 1byte씩 4번 돌게 되는 것이다
		// 맨 오른쪽 1byte 꺼내와서, 계산
		// 8bit ( 1byte ) 오른쪽 밀고
		// 또 맨 오른쪽 계산.
		// 제일 끝에 1byte씩을 꺼내오는 것이다
		// 따라서 unsigned char , 즉, 1byte를 꺼내오는 것이다
		// 
		for (size_t i = 0; i < Length; i++)
		{
			// ff : 8byte --> 16진수 하나가 2진수 4개표현 --> 4bit 표현 
			unsigned char data = NewKey & 0xff;
			m_HashKey += data;
			NewKey >>= 8;
		}
		return m_HashKey;
	}

	// 템플릿 특수화
	// 템플릿의 인자 타입에 따라 
	// 별도의 처리함수를 만들어놓고 작업할 수 있게 해준다
	template<>
	unsigned __int64 GetHash(std::string key)
	{
		m_HashKey = 0;
		size_t Length = key.length();
		for (size_t i = 0; i < Length; i++)
		{
			// 짝수 번째 항목은 더해주고
			if (i % 2 == 0)
				m_HashKey += (unsigned __int64)key[i];
			// 홀수 번째 항목은 곱해주고 
			else
				m_HashKey *= (unsigned __int64)key[i];
		}
		return m_HashKey;
	}

	template<>
	unsigned __int64 GetHash(const char* key)
	{
		m_HashKey = 0;
		// size_t 
		// 32 bit : default
		// __int64 : 64bit int로 잡힌다. 
		// 즉, 32bit 개발단계에서는 32 bit 
		// 64bit 개발단계에서는 64bit 
		// 쉽게 말해 unsigned int의 typedef 형태
		// 메모리나 , 문자열의 길이를 구할 때에는 
		// size_t 라는 형으로 길이가 반환된다. 
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