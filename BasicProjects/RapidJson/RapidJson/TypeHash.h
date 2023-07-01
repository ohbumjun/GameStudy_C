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
			// 2) TypeName<type_name_prober>() == "void" : ���� void template Ư��ȭ �Լ��� ȣ���Ѵ�.
			// �ش� string ���� TypeName<type_name_prober>() �� ù��° index �� �����Ѵ�
			// 3) ����� : TypeName<type_name_prober>() �� void �̹Ƿ�, �� ������ 2��° void �� ���� ��ġ�� �����ϰ� �ȴ�.
			return wrapped_type_name<type_name_prober>().find(TypeName<type_name_prober>());
		}

		constexpr std::size_t wrapped_type_name_suffix_length()
		{
			// ex. 123 (�� ����) - 112(void �� ó�� ������ġ) - 4 (void ����)
			// "(void)" ��ŭ�� ���� 
			return wrapped_type_name<type_name_prober>().length()
				- wrapped_type_name_prefix_length()
				- TypeName<type_name_prober>().length();
		}
	}

	template <typename T>
	constexpr std::string_view TypeName()
	{
		// T �� int �� ��� class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Reflection::detail::wrapped_type_name<int>(void)
		constexpr  auto wrapped_name = detail::wrapped_type_name<T>();

		// wrapped_type_name<type_name_prober>() ?
		//  : wrapped_type_name<type_name_prober>() : class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Reflection::detail::wrapped_type_name<void>(void)
		// ��� : 112
		constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();

		// ��� : 7
		constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();

		// wrapped_name ���� <> �ȿ� �ִ� "int" ��ŭ�� ���� == 3
		constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;

		// <> �ȿ� �ִ� "int" �� ��"
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
		// TODO : unordered_map ���� ���� caching �� �صδ� �������� �����ϱ�
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
// remove_all_pointers ��� ����ü�� member "Type" �� template ���ڷ� ���� "T" type �� �����Ѵ�. 
template <typename T> struct remove_all_pointers
{
	using Type = T;
};

// T* Ÿ�Կ� ���� ���ø� Ư��ȭ �̴�. ����������, ���ø� ���ڷ� ������ Ÿ���� ��������
// �̿� ���� remove_all_pointers ����ü�� member "Type" �� �����͸� ������ ���� T Type �� �����Ѵ�. 
template <typename T> struct remove_all_pointers<T*>
{
	using Type = remove_all_pointers<T>::Type;
};

// ���������� remove_all_pointers_t �� remove_all_pointers ����ü�� member "Type". ��, ���ڷ� ���� Type �״��
// �� ����Ű���� �����Ѵ�. 
template <typename T>
using remove_all_pointers_t = remove_all_pointers<T>::Type;
