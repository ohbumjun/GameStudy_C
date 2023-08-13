#pragma once

#include <memory>
#include <vector>

namespace bbe
{
	template<typename T>
	T nextMultiple(T multipleOf, T value)
	{
		T multiple = value + multipleOf - 1;
		multiple -= (multiple % multipleOf);
		return multiple;
	}

	class StackAllocatorDestructor {
		private:
			const void* m_data;
			void(*destructor)(const void*); // 함수 포인터 
		public :
			template<typename T>
			explicit StackAllocatorDestructor(const T& data) : m_data(std::addressof(data))
		{
				destructor = [](const void* lamdaData)
				{
					auto originalType = static_cast<const T*>(lamdaData);
					originalType->~T();
				};
		}
			void operator () ()
			{
				destructor(m_data);
			}
	};

	typedef unsigned char byte;

	class StackAllocatorMarker {
	public:
		byte* m_markerValue;
		size_t m_destructorHandle;

		StackAllocatorMarker(byte* markerValue, size_t destructorHandle) :
		m_markerValue(markerValue), m_destructorHandle(destructorHandle)
		{
			
		}
	};

	class StackAllocator {
	private:
		static const size_t STACKALLOCATORDEFAULTSIZE = 1024;
		byte* m_data = nullptr;
		byte* m_head = nullptr;
		size_t m_size = 0;

		std::vector<StackAllocatorDestructor> destructors;

		// is_trivially_destructible : 소멸 가능한 객체인지 확인한다.
		template<typename T>
		inline typename std::enable_if<std::is_trivially_destructible<T>::value>::type
			addDestructorToList(T* object)
		{
			// Do Nothing
		}

		// 소멸가능하지 않다면...?
		template<typename T>
		inline typename std::enable_if<!std::is_trivially_destructible<T>::value>::type
			addDestructorToList(T* object)
		{
			destructors.push_back(StackAllocatorDestructor(*object));
		}

	public :
		explicit StackAllocator(size_t size = STACKALLOCATORDEFAULTSIZE) : m_size(size)
	{
			m_data = new byte[m_size];
			m_head = m_data;
	}

		~StackAllocator()
	{
			if (m_data != m_head)
			{
				__debugbreak();
				return;
			}

			delete[] m_data;
			m_data = nullptr;
			m_head = nullptr;
	}
		StackAllocator(const StackAllocator& other) = delete;   // 복사 생성
		StackAllocator(const StackAllocator&& other) = delete; // 이동 생성
		StackAllocator& operator = (const StackAllocator& other) = delete; // 복사 대입
		StackAllocator& operator = (const StackAllocator&& other) = delete; // 이동 대입

		void* allocator(size_t amountOfBytes, size_t alignment = 1)
		{
			byte* allocationLocation = (byte*)nextMultiple(alignment, (size_t)m_head);
			byte* newHeadPointer = allocationLocation + amountOfBytes;

			// 범위를 넘어가지 않을 때
			if (newHeadPointer <= m_data + m_size)
			{
				m_head = newHeadPointer;
				return allocationLocation;
			}
			else
			{
				// Todo ad fur
				return nullptr;
			}
		}

		template<typename T, typename...Arguments>
		T* allocateObject(size_t amountOfObjects = 1, Arguments&& ...args)
		{
			// alignof -> sizeof 와 비슷한 기능 
			byte* allocationLocation = (byte*)nextMultiple(alignof(T), (size_t)m_head);
			byte* newHeadPointer = allocationLocation + amountOfObjects * sizeof(T);

			if (newHeadPointer <= m_data + m_size)
			{
				T* returnPointer = reinterpret_cast<T*>(allocationLocation);
				m_head = newHeadPointer;
				for (size_t i = 0; i < amountOfObjects; ++i)
				{
					T* object = new (std::addressof(returnPointer[i])) T(std::forward<Arguments>(args)...);
					// destructors.push_back(StackAllocatorDestructor(*object));
					addDestructorToList(*object);
				}

				return returnPointer;
			}
			else
			{
				return nullptr;
			}
		}

		StackAllocatorMarker getMarker()
		{
			return StackAllocatorMarker(m_head, destructors.size());
		}

		void deallocateToMarker(StackAllocatorMarker sam)
		{
			m_head = sam.m_markerValue;

			while (destructors.size() > sam.m_destructorHandle)
			{
				destructors.back()();
				destructors.pop_back();
			}
		}

		void deallocateAll()
		{
			m_head = m_data;
			while (destructors.size() > 0)
			{
				destructors.back()();
				destructors.pop_back();
			}
		}
	};
}
