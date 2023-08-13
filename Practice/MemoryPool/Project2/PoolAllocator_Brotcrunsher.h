#pragma once

#include <memory>

namespace bbe
{
	template<typename T>
	union PoolChunk
	{
		T value;
		PoolChunk<T>* nextPoolChunk;

		PoolChunk(){}
		~PoolChunk(){}
	};

	// std::allocator -> 불필요한 초기화 방지 
	// new = 메모리 할당 + 초기화(필수)
	// allocator = 1) 메모리 할당 --> 초기화 되지 않은 상태의 메모리 시작주소를 얻을 수 있다.
	// (원래 초기화되지 않은 메모리 공간에 객체를 직접 할당할 수 없다. 하지만, 해당 클래스의 멤버 함수
	// 또는 관련 함수가 초기화 되지 않은 공간에 객체를 저장할 수 있도록해준다 )

	// 2) 할당받은 메모리에 객체를 생성 후, 메모리 해제(dealloacte) 없이 생성한 객체들을
	// 소멸(destroy) 시킬 수 있다.
	// 즉, 메모리의 재할당 없이 그 공간을 allocate 했던 초기 상태로 만들 수 있다.
	// new 연산자로 할당한 메모리 공간은 delete 를 사용하면, 메모리 공간이 해제가 된다.
	// 즉, 하나의 과정을 세밀하게 나누어 컨트롤 할 수 있는 것이다.

	// 소멸과, 메모리 해제는 다르다. 
	// 
	template<typename T>
	class PoolAllocator
	{
	private :
		static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
		size_t m_size = 0;

		PoolChunk<T>* m_data = nullptr;
		PoolChunk<T>* m_AllocPointer = nullptr;

		int m_openAllocations = 0;

	public :
		explicit PoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE) :
			m_size(size)
		{
			m_data = new PoolChunk<T>[size];
			m_AllocPointer = m_data;

			for (size_t i = 0; i < m_size - 1; ++i)
			{
				m_data[i].nextPoolChunk = std::addressof(m_data[i + 1]);
			}

			m_data[m_size - 1].nextPoolChunk = nullptr;
		}

		PoolAllocator(const PoolAllocator& other) = delete;   // 복사 생성
		PoolAllocator(const PoolAllocator&& other) = delete; // 이동 생성
		PoolAllocator& operator = (const PoolAllocator& other) = delete; // 복사 대입
		PoolAllocator& operator = (const PoolAllocator&& other) = delete; // 이동 대입

		~PoolAllocator()
		{
			if (m_openAllocations != 0)
			{
				__debugbreak();
			}

			delete [] m_data;
			m_data = nullptr;
			m_AllocPointer = nullptr;
		}

		template<typename ...Arguments>
		T* allocate(Arguments&& ... args)
		{
			if (m_AllocPointer == nullptr)
			{
				// 새로 다시 할당하기 
				// __debugbreak();
				allocateBlock(args...);
				return nullptr;
			}

			m_openAllocations++;

			PoolChunk<T>* poolChunk = m_AllocPointer;

			// 만약, Pool 내의 모든 메모리 공간을 활용한다면
			// m_AllocPointer 은 Null 이 되고
			// 새로운 Block 을 할당하게 된다.
			m_AllocPointer = m_AllocPointer->nextPoolChunk;

			T* retVal = new (std::addressof(poolChunk->value)) T(std::forward<Arguments>(args)...);

			return retVal;
		}

		template<typename ...Arguments>
		void allocateBlock(Arguments&& ... args)
		{
			std::cout << "Allocate New Block" << std::endl;

			PoolChunk<T>* Newdata = new PoolChunk<T>[m_size];
			m_AllocPointer = Newdata;

			for (size_t i = 0; i < m_size - 1; ++i)
			{
				Newdata[i].nextPoolChunk = std::addressof(m_data[i + 1]);
			}

			Newdata[m_size - 1].nextPoolChunk = nullptr;
		}

		void deallocate(T* data)
		{
			// 소멸자 호출시켜주기 
			data->~T();
			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
			poolChunk->nextPoolChunk = m_AllocPointer;
			m_AllocPointer = poolChunk;

			m_openAllocations--;
		}
	};
}