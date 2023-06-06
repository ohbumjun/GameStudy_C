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

// final : ���̻� ������� �ʴ� ������ class
// ���⿡ �� Type �� ���� ��� ������ ���� �Ǿ��ִ�.
struct TypeInfo final
{
	std::string_view Name{};
	uint32_t Size{};

	// alignof(T) : �ش� type �� address �� � ������ ����� �Ǿ�� �ϴ���
	// ex) 64 bit������ "double" �� aligntment �� 8�̴�., "int" �� 4
	// ��, double ������ �ּҴ� 8�� ������� �Ѵٴ� ���̴�.
	/*
		struct MyStruct {
			int x;
			double y;
		};

		alignof(MyStruct) �� 8�� �ȴ�.
		����, ����Ʈ�е��� ���� ���� ū �ڷ����� �������� �����ϴ� �� ����.
		double �� �����ϹǷ� 8�� �ȴ�.
		���� �̷��� alignof ���� �÷���, �����Ϸ� � ���� �޶�����.
	*/
	uint32_t Align{};

	std::bitset<TypeFlags_SIZE> Flags{};

	// ������ ����ϴ� ��� ���� �����ϱ� 
	// - GameComponent �� ��ӹ޴� Class ���� �����ڸ� ���� ������� �� �ִ�.
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

	// memcpy  �� ���� �Լ��� ���ؼ� copy �� �����ϸ� trivially_copyable
	/*
	false �� ��� ?

	struct NonTriviallyCopyable {
		NonTriviallyCopyable() {}
		NonTriviallyCopyable(const NonTriviallyCopyable&) {}
		~NonTriviallyCopyable() {}
	};

	userdefined constructor, copy constructor, and destructor �� �ִ� ���
	Ȥ�� �׷��� ����� ��� �ִ� ���
	*/
	info.Flags[TypeFlags_IsTriviallyCopyable] = std::is_trivially_copyable_v<T>;
	info.Flags[TypeFlags_IsIntegral] = std::is_integral_v<T>;
	info.Flags[TypeFlags_IsFloatingPoint] = std::is_floating_point_v<T>;
	info.Flags[TypeFlags_IsFundamental] = std::is_fundamental_v<T>;

	// GameComponent �� ��ӹ��� ����̶�� ������ �Լ��� �������ش�. 
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
	// ���� ���� �ϴ� �� ��ſ� StaticData ��� struct �ȿ� �������� ���� ?
	// static �����̹Ƿ�, � ������ �ʱ�ȭ �Ǵ��� ��Ʈ�� �� �� ����.
	// ������ �Ʒ��� ���� �����ϸ� GetStatics() �� ȣ���ϴ� ���� �ʱ�ȭ�ǵ��� 
	// �� �� �ִ�.
	// ���� Application �� ��� RegisterTypeId() �ȿ��� GetStatics() �� ȣ���Ѵ�.
	// ��, REgisterTypeID �Լ��� static �̴�.
	// �ش� �Լ��� ȣ��� �� unordered_map �� �ݵ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	// �̷��� ������ �����ϱ� ���� ��ġ�̴�. 
	struct StaticData
	{
		// TypeId �� Id ~ TypeInfo
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
	// �Ʒ��� static ������ ����Ǿ��� ������, �� Type ���� �ѹ����� RegisterTypeOnce �� ���� ������ ȣ��
	// T��, �� type �� ���� �ѹ����� ypeId::RegisterTypeId<T>() �� ȣ��� ���̴�.
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

// VARNAME##TYPE �� ���� �̸��� "����"�� �����.
#define _REGISTER_TYPE_INTERNAL(TYPE,VARNAME) RegisterType<TYPE> VARNAME##TYPE{};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)

// REGISTER_TYPE(int) ==
// RegisterType<int>RegisterType_int{}

//REGISTER_TYPE(int) �� �ش��ϴ� �ڵ带, �� Class ���� ��� �ۼ������ �Ѵ�.
// main.cpp ���� ���� �ۼ����൵ �ǰ�, �װ��� �˾Ƽ�
// ���󿡼��� main.cpp �� �ٷ� ������
// ������ static ���� �����̹Ƿ�, ��� �����൵ �������.

template<typename T>
inline constexpr TypeId TypeId::Create()
{
	// �ش� Type �� map �� ����Ѵ�. 
	RegisterType<T> RegisterType{};

	return TypeId(Reflection::TypeId<T>());
}

constexpr bool operator==(TypeId lhs, TypeId rhs)
{
	return lhs.GetId() == rhs.GetId();
}

