#pragma once

#include <vector>

namespace bbe
{
	typedef unsigned char byte;

	template<typename T>
	T nextMultiple(T multipleOf, T value)
	{
		T multiple = value + multipleOf - 1;
		multiple -= (multiple % multipleOf);
		return multiple;
	}

	class StackAllocatorDestructor
	{
	private :
		const void* m_Data;
		void (*destructor)(const void*);

	public :
		template<typename T>
		explicit StackAllocatorDestructor(const T& data) : 
			m_Data(std::addressof(data))
		{
			destructor = [](const void* lamdaData) {
				auto originalType = static_cast<const T*>(lamdaData);
				originalType->~T();
			}
		}

		void operator () ()
		{
			destructor(m_Data);
		}
	};

	class StackAllocator
	{
	private :
		static const size_t STACKALLOCATORDEFAULTSIZE = 1024;
		byte* m_Data = nullptr;
		byte* m_Head = nullptr;
		size_t m_Size = 0;
		
		std::vector<StackAllocatorDestructor> destructors;

		// is_trivially_destructible 
		// - 해당 형식이 소멸 가능한지를 체크한다.

		template<typename T>
		inline typename std::enable_if<std::is_trivially_destructible<T>::value>::type 
			addDestructorToList(T* Object)
		{
			//do nothing
		}

		template<typename T>
		inline typename std::enable_if<!std::is_trivially_destructible<T>::value>::type
			addDestructorToList(T* Object)
		{
			destructors.push_back(StackAllocatorDestructor(*object));
		}

	public :
		explicit StackAllocator(size_t size = STACKALLOCATORDEFAULTSIZE) : m_Size(size){
			m_Data = new byte[m_Size];
			m_Head = m_Data;
		}
		~StackAllocator()
		{
			if (m_Data != m_Head)
			{
				// TODO : add further error handling
				__debugbreak(); 
			}

			delete m_Data;
			m_Data = nullptr;
			m_Head = nullptr;
		}

		StackAllocator(const StackAllocator& other)						 = delete;
		StackAllocator(StackAllocator& other)								 = delete;
		StackAllocator& operator = (const StackAllocator& other)	 = delete;
		StackAllocator& operator = (StackAllocator& other)			 = delete;

		void* allocate(size_t amountOfBytes, size_t alignment = 1)
		{
			byte* allocationLocation = (byte*)nextMultiple(alignment, (size_t)m_Head);
			byte* newHeaderPointer = allocationLocation + amountOfBytes;

			
			if (newHeaderPointer <= m_Data + m_Size)
			{
				m_Head = newHeaderPointer;
				return allocationLocation;
			}
			else
			{
				// 추가 메모리 필욘
				// TODO : add further error handling
				return nullptr;
			}
		}

		template<typename T, typename ...arguments>
		T* allocateObjects(size_t amountOfObjects = 1, arguments&& ...args)
		{
			// alignof : size_t 값으로 바이트 단위로 반환한다.
			byte* allocationLocation = (byte*)nextMultiple(alignof(T), (size_t)m_Head);
			byte* newHeadPointer = allocationLocation + amountOfObjects * sizeof(T);

			if (newHeadPointer <= m_Data + m_Size)
			{
				T* returnPointer = reinterpret_cast<T*>(allocationLocation);

				m_Head = newHeadPointer;

				for (size_t i = 0; i < amountOfObjects; ++i)
				{
					T* object = new (std::addressof(returnPointer[i])) T(std::forward<arguments>(args)...);
					addDestructorToList(object);
				}

				return returnPointer;
			}
			else
			{
				// TODO add furthre error handling
				return nullptr;
			}
		}

		void deallocateAll()
		{
			m_Head = m_Data;
			while (destructors.size() > 0)
			{
				destructors.back()();
				destructors.pop_back();
			}
		}
	};
}