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

	// std::allocator -> ���ʿ��� �ʱ�ȭ ���� 
	// new = �޸� �Ҵ� + �ʱ�ȭ(�ʼ�)
	// allocator = 1) �޸� �Ҵ� --> �ʱ�ȭ ���� ���� ������ �޸� �����ּҸ� ���� �� �ִ�.
	// (���� �ʱ�ȭ���� ���� �޸� ������ ��ü�� ���� �Ҵ��� �� ����. ������, �ش� Ŭ������ ��� �Լ�
	// �Ǵ� ���� �Լ��� �ʱ�ȭ ���� ���� ������ ��ü�� ������ �� �ֵ������ش� )

	// 2) �Ҵ���� �޸𸮿� ��ü�� ���� ��, �޸� ����(dealloacte) ���� ������ ��ü����
	// �Ҹ�(destroy) ��ų �� �ִ�.
	// ��, �޸��� ���Ҵ� ���� �� ������ allocate �ߴ� �ʱ� ���·� ���� �� �ִ�.
	// new �����ڷ� �Ҵ��� �޸� ������ delete �� ����ϸ�, �޸� ������ ������ �ȴ�.
	// ��, �ϳ��� ������ �����ϰ� ������ ��Ʈ�� �� �� �ִ� ���̴�.

	// �Ҹ��, �޸� ������ �ٸ���. 
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

		PoolAllocator(const PoolAllocator& other) = delete;   // ���� ����
		PoolAllocator(const PoolAllocator&& other) = delete; // �̵� ����
		PoolAllocator& operator = (const PoolAllocator& other) = delete; // ���� ����
		PoolAllocator& operator = (const PoolAllocator&& other) = delete; // �̵� ����

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
				// ���� �ٽ� �Ҵ��ϱ� 
				// __debugbreak();
				allocateBlock(args...);
				return nullptr;
			}

			m_openAllocations++;

			PoolChunk<T>* poolChunk = m_AllocPointer;

			// ����, Pool ���� ��� �޸� ������ Ȱ���Ѵٸ�
			// m_AllocPointer �� Null �� �ǰ�
			// ���ο� Block �� �Ҵ��ϰ� �ȴ�.
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
			// �Ҹ��� ȣ������ֱ� 
			data->~T();
			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
			poolChunk->nextPoolChunk = m_AllocPointer;
			m_AllocPointer = poolChunk;

			m_openAllocations--;
		}
	};
}