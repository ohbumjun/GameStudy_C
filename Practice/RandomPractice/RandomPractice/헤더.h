#pragma once

template<typename T>
class Vector
{
public :
	Vector() {
		m_Array = new T[m_Capacity];
	};
private :
	int m_Size = 0;
	int m_Capacity = 8;
	T* m_Array;
public :
	void push_back(const T& data)
	{
		if (m_Size >= m_Capacity)
		{
			m_Capacity *= 2;
			T* NewArray = new T[m_Capacity];
			memcpy(NewArray, m_Array, sizeof(m_Array));
			delete [] m_Array;
			m_Array = NewArray;
		}
		m_Array[m_Size] = data;
		++m_Size;
	}
	void erase(const T& data)
	{
		int EraseIdx = -1;
		for (int i = 0; i < m_Size; ++i)
		{
			if (m_Array[i] == data)
			{
				EraseIdx = i;
				break;
			}
		}

		eraseIndex(EraseIdx);
	}
	void eraseIndex(int idx)
	{
		if (idx == -1 || idx >= m_Size)
			return;

		for (int i = idx; i < m_Size; ++i)
		{
			m_Array[i] = m_Array[i + 1];
		}

		--m_Size;
	}
	T& operator [] (int idx)
	{
		if (idx < 0 || idx >= m_Size)
			assert(false);

		return m_Array[idx];
	}
};