#pragma once

#include "TypeHash.h"

#include <bitset>
#include <cassert>
#include <functional>

enum TypeFlags
{
	TypeFlags_IsFundamental,
	TypeFlags_IsIntegral,
	TypeFlags_IsFloatingPoint,
	TypeFlags_IsTriviallyCopyable,
	TypeFlags_SIZE // MAX
};

class GameComponent
{
public:
	GameComponent() = default;
	virtual ~GameComponent() = default;

public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Copy() {};
	virtual void Serialize() {};
};

// final : 더이상 상속하지 않는 마지막 class
// 여기에 각 Type 에 대한 모든 정보가 들어가게 되어있다.
struct TypeInfo final
{
	std::string_view Name{};
	uint32_t Size{};

	// alignof(T) : 해당 type 의 address 가 어떤 숫자의 배수가 되어야 하는지
	// ex) 64 bit에서는 "double" 의 aligntment 는 8이다., "int" 는 4
	// 즉, double 변수의 주소는 8의 배수여야 한다는 것이다.
	/*
		struct MyStruct {
			int x;
			double y;
		};

		alignof(MyStruct) 는 8이 된다.
		뭔가, 바이트패딩과 같이 가장 큰 자료형을 기준으로 세팅하는 것 같다.
		double 을 포함하므로 8이 된다.
		물론 이러한 alignof 값도 플랫폼, 컴파일러 등에 따라 달라진다.
	*/
	uint32_t Align{};

	std::bitset<TypeFlags_SIZE> Flags{};

	// 생성자 등록하는 방법 따로 참고하기 
	// - GameComponent 를 상속받는 Class 들은 생성자를 따로 만들어줄 수 있다.
	std::function<GameComponent* ()> GCConstructor{};

	template<typename T>
	static constexpr TypeInfo Create();
};

template<typename T>
inline constexpr TypeInfo TypeInfo::Create()
{
	TypeInfo info{}

	info.Name = Reflection::TypeName<T>();
	info.Size = sizeof(T);
	info.Align = alignof(T);

	// memcpy  와 같은 함수를 통해서 copy 가 가능하면 trivially_copyable
	/*
	false 인 경우 ?

	struct NonTriviallyCopyable {
		NonTriviallyCopyable() {}
		NonTriviallyCopyable(const NonTriviallyCopyable&) {}
		~NonTriviallyCopyable() {}
	};

	userdefined constructor, copy constructor, and destructor 가 있는 경우
	혹은 그러한 멤버를 들고 있는 경우
	*/
	info.Flags[TypeFlags_IsTriviallyCopyable] = std::is_trivially_copyable_v<T>;
	info.Flags[TypeFlags_IsIntegral] = std::is_integral_v<T>;
	info.Flags[TypeFlags_IsFloatingPoint] = std::is_floating_point_v<T>;
	info.Flags[TypeFlags_IsFundamental] = std::is_fundamental_v<T>;

	// GameComponent 를 상속받은 대상이라면 생성자 함수를 세팅해준다. 
	if constexpr (std::is_base_of_v<GameComponent, T>)
	{
		info.GCConstructor = []()->GameComponent*
		{
			return new T();
		}
	}

	return info;
}


class TypeId final
{
public:
	constexpr TypeId() = default;
	constexpr TypeId(uint64_t id) : ID{ id } {};
public:
	template<typename T>
	static constexpr TypeId Create();

public:
	constexpr void			SetTypeId(uint64_t typeId) { ID = typeId; }
	constexpr uint64_t		GetId()	const { return ID; }
	const TypeInfo& GetTypeInfo()	const { return GetTypeInformation(*this); }

	const std::string_view	GetTypeName()	const { return GetTypeInfo().Name; }
	uint32_t				GetTypeSize()	const { return GetTypeInfo().Size; }
	uint32_t				GetTypeAlignment()	const { return GetTypeInfo().Align; }

	bool					IsFundamental()	const { return GetTypeInfo().Flags[TypeFlags_IsFundamental]; }
	bool					IsIntegral()	const { return GetTypeInfo().Flags[TypeFlags_IsIntegral]; }
	bool					IsFloatingPoint()	const { return GetTypeInfo().Flags[TypeFlags_IsFloatingPoint]; }
	bool					IsTriviallyCopyable()	const { return GetTypeInfo().Flags[TypeFlags_IsTriviallyCopyable]; }

public:
	static const TypeInfo& GetTypeInformation(TypeId typeID)
	{
		assert(GetStatics().TypeInfos.find(typeID.GetId()) != GetStatics().TypeInfos.end());
		return GetStatics().TypeInfos[typeID.GetId()];
	}

	static const auto& GetAllTypeInformation()
	{
		return GetStatics().TypeInfos;
	}

	template<typename T>
	static TypeInfo& RegisterTypeId()
	{
		auto& typeInfos = GetStatics().TypeInfos;
		const uint64_t typeId = TypeId::Create<T>().GetId();

		assert(GetAllTypeInformation().find(typeId) != GetAllTypeInformation().end());

		GetStatics().TypeInfos.emplace(
			typeId,
			TypeInfo::Create<T>()
		);

		return
	}

	bool operator==(const TypeId& other) const {
		return ID == other.ID;
	}

	// bool operator!=(const TypeId& other) const {
	// 	return ID != other.ID;
	// }

private:
	// inline static std::unordered_map<uint64_t, TypeInfo> TypeInfos{};
	// 위와 같이 하는 것 대신에 StaticData 라는 struct 안에 선언해준 이유 ?
	// static 변수이므로, 어떤 시점에 초기화 되는지 컨트롤 할 수 없다.
	// 하지만 아래와 같이 세팅하면 GetStatics() 를 호출하는 순간 초기화되도록 
	// 할 수 있다.
	// 현재 Application 의 경우 RegisterTypeId() 안에서 GetStatics() 를 호출한다.
	// 즉, REgisterTypeID 함수도 static 이다.
	// 해당 함수가 호출될 때 unordered_map 이 반드시 초기화되어 있어야 한다.
	// 이러한 순서를 보장하기 위한 장치이다. 
	struct StaticData
	{
		// TypeId 의 Id ~ TypeInfo
		std::unordered_map<uint64_t, TypeInfo> TypeInfos{};
	};

	static StaticData& GetStatics()
	{
		static StaticData data{};
		return data;
	}
private:
	uint64_t ID{};
};

template<typename T>
class RegisterType
{
private:
	// 아래의 static 변수로 선언되었기 때문에, 각 Type 마다 한번씩만 RegisterTypeOnce 에 대한 생성자 호출
	// T즉, 각 type 에 대해 한번씩만 ypeId::RegisterTypeId<T>() 이 호출될 것이다.
	class RegisterTypeOnce
	{
	public:
		RegisterTypeOnce()
		{
			TypeId::RegisterTypeId<T>();
		}
	};

	inline static RegisterTypeOnce Registerer{};
};

// VARNAME##TYPE 을 붙인 이름의 "변수"로 만든다.
#define _REGISTER_TYPE_INTERNAL(TYPE,VARNAME) RegisterType<TYPE> VARNAME##TYPE{};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)

// REGISTER_TYPE(int) ==
// RegisterType<int>RegisterType_int{}

//REGISTER_TYPE(int) 에 해당하는 코드를, 각 Class 별로 모두 작성해줘야 한다.
// main.cpp 실행 전에 작성해줘도 되고, 그것은 알아서
// 영상에서는 main.cpp 에 바로 적어줌
// 어차피 static 변수 형태이므로, 어디에 적어줘도 상관없다.

template<typename T>
inline constexpr TypeId TypeId::Create()
{
	// 해당 Type 을 map 에 등록한다. 
	RegisterType<T> RegisterType{};

	return TypeId(Reflection::TypeId<T>());
}

constexpr bool operator==(TypeId lhs, TypeId rhs)
{
	return lhs.GetId() == rhs.GetId();
}

