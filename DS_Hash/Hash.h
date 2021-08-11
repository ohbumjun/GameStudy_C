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
	// unsigened : 양수 only
	unsigned __int64 m_HashKey;

public :
	// 템플릿 특수화 
	template<typename KEY>
	unsigned __int64 GetHash(KEY key)
	{
		// 다양한 type의 key에 대한
		// Hash함수를 만들 것이다 
		
		// 최대한 key들끼리
		// 충돌을 적게 나게 하는 알고리즘 파트가
		// 바로 이 부분이다.
		
		// 시작시 0으로 만들기
		m_HashKey = 0;
		size_t = Length = sizeof(key);
		unsigned __int64 NewKey = (unsigned __int64)key;

		for (size_t i = 0; i < Length; i++)
		{
			// 오른쪽으로 계속 밀면서
			// 1byte, 8bit씩 꺼내올 것이다

			// 맨 오른쪽 bit 요소를 가져오게 될 것이다
			// 1111111 & 연산하게 되면
			// 
			unsigned char data = NewKey & 0xff;
		
			// 1byte씩 모두 더한다
			m_HashKey += data;
			// 오른쪽으로 8bit 민다
			NewKey >>= 8;
		}
		return m_HashKey;
	}

		// 템플릿 특수화란 ?
		// 들어오는 type에 따라서
		// 특수화된 함수를 호출할 수 있다
		// type에 따라
		// 별도의 처리함수를 만들어놓고
		// 작업할 수 있게 해준다

		// string type이 인자로 들어오게 되면
		// 아래 함수가 실행된다.
		template<>
		unsigned __int64 GetHash(std::string key)
		{
			m_HashKey = 0;
			size_t Length = key.length();
			for (size_t i = 0; i < Length; i++)
			{
				if (i % 2 == 0)
					// 아스키 코드값 더해주기
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
			
			// strlen : 문자열 길이 구해주는 함수
			// size_t : unsigned int : 원래는 4byte
			// 만일 --> __int64로 해놓으면, 8byte
			size_t Length = strlen(key);
			
			for (size_t i = 0; i < Length; i++)
			{
				if (i % 2 == 0)
					// 아스키 코드값 더해주기
					m_HashKey += (unsigned __int64)key[i];
				else
					m_HashKey *= (unsigned __int64)key[i];
			}
			return m_HashKey;
		}

};

