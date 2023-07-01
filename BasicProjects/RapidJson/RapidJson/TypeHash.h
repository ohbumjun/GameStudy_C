#pragma once

#include <string_view>
#include <cstdint>
#include <array>
#include <functional>
#include <type_traits>

namespace Reflection
{
	//https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way
	//asnwer by einpoklum

	template <typename T> constexpr std::string_view TypeName();

	template <>
	constexpr std::string_view TypeName<void>()
	{
		return "void";
	}

	namespace detail
	{
		using type_name_prober = void;

		template <typename T>
		constexpr std::string_view wrapped_type_name()
		{
#ifdef __clang__
			return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
			return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
			return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
		}

		constexpr std::size_t wrapped_type_name_prefix_length()
		{
			// std::cout << "TypeName<type_name_prober>() : " << TypeName<type_name_prober>() << std::endl;
			// std::cout << "wrapped_type_name<type_name_prober>() : " << wrapped_type_name<type_name_prober>() << std::endl;
			
			// 1) wrapped_type_name<type_name_prober>() ?
			// -> wrapped_type_name<type_name_prober>() : class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Reflection::detail::wrapped_type_name<void>(void)
			// 2) TypeName<type_name_prober>() == "void" : 위의 void template 특수화 함수를 호출한다.
			// 해당 string 에서 TypeName<type_name_prober>() 의 첫번째 index 를 리턴한다
			// 3) 결과값 : TypeName<type_name_prober>() 는 void 이므로, 맨 끝에서 2번째 void 의 시작 위치를 리턴하게 된다.
			return wrapped_type_name<type_name_prober>().find(TypeName<type_name_prober>());
		}

		constexpr std::size_t wrapped_type_name_suffix_length()
		{
			// ex. 123 (총 길이) - 112(void 맨 처음 시작위치) - 4 (void 길이)
			// "(void)" 만큼의 길이 
			return wrapped_type_name<type_name_prober>().length()
				- wrapped_type_name_prefix_length()
				- TypeName<type_name_prober>().length();
		}
	}

	template <typename T>
	constexpr std::string_view TypeName()
	{
		// T 가 int 일 경우 class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Reflection::detail::wrapped_type_name<int>(void)
		constexpr  auto wrapped_name = detail::wrapped_type_name<T>();

		// wrapped_type_name<type_name_prober>() ?
		//  : wrapped_type_name<type_name_prober>() : class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Reflection::detail::wrapped_type_name<void>(void)
		// 결과 : 112
		constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();

		// 결과 : 7
		constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();

		// wrapped_name 에서 <> 안에 있는 "int" 만큼의 길이 == 3
		constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;

		// <> 안에 있는 "int" 의 시"
		return wrapped_name.substr(prefix_length, type_name_length);
	}


	/**
	* HASHING
	*/

	constexpr uint64_t Hash(std::string_view str)
	{
		std::uint64_t hash_value = 0xcbf29ce484222325ULL;
		constexpr std::uint64_t prime = 0x100000001b3ULL;
		for (char c : str)
		{
			hash_value ^= static_cast<std::uint64_t>(c);
			hash_value *= prime;
		}
		return hash_value;
	}

	/**
	* TYPE ID
	*/

	template <typename Type>
	constexpr uint64_t TypeId()
	{
		// TODO : unordered_map 같은 곳에 caching 을 해두는 방향으로 진행하기
		return Hash(TypeName<Type>());
	}
}

template<typename T>
constexpr uint32_t CountPointers(uint32_t counter = 0)
{
	if (constexpr std::is_pointer_v<T>)
		return CountPointers<std::remote_pointer_t<T>>(++counter);
	else
		return counter;
};

// how to remove all pointers of type
// remove_all_pointers 라는 구조체는 member "Type" 에 template 인자로 들어온 "T" type 을 세팅한다. 
template <typename T> struct remove_all_pointers
{
	using Type = T;
};

// T* 타입에 대한 템플릿 특수화 이다. 마찬가지로, 템플릿 인자로 포인터 타입이 들어오더라도
// 이에 대한 remove_all_pointers 구조체는 member "Type" 에 포인터를 제거한 원본 T Type 을 세팅한다. 
template <typename T> struct remove_all_pointers<T*>
{
	using Type = remove_all_pointers<T>::Type;
};

// 최종적으로 remove_all_pointers_t 는 remove_all_pointers 구조체의 member "Type". 즉, 인자로 들어온 Type 그대로
// 를 가리키도록 세팅한다. 
template <typename T>
using remove_all_pointers_t = remove_all_pointers<T>::Type;
