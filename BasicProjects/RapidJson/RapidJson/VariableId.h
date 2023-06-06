#pragma once

#include "TypeInfo.h"
#include <string>

// void Function(int var0, float* var1, const string& var2)
// 이와 같이 함수 , 그리고 그 함수와 관련된 variable 에 대해 describe 해야할 필요가 있다.
// 보통 변수는 아래와 같이 구성된다
// >> const volatile int (&, &&, *, [])
// const volatile : Qualifiers
// int            : type
// (&, &&, *, []) : Modifier

class VariableId final
{
private:
	//const, volatile, &, &&
	static constexpr uint32_t ConstFlag = 1 << 0;
	static constexpr uint32_t ReferenceFlag = 1 << 1;
	static constexpr uint32_t VolatileFlag = 1 << 2;
	static constexpr uint32_t RValReferenceFlag = 1 << 3;

public:
	constexpr explicit VariableId(TypeId id) : Type{ id } {};
	constexpr VariableId() = default;

	template <typename T>
	static constexpr VariableId Create();

public:
	constexpr TypeId GetTypeId() const { return Type; }
	constexpr void SetTypeId(TypeId id) { Type = id; }

	constexpr void SetConstFlag() { TraitFlags |= ConstFlag; }
	constexpr void SetReferenceFlag() { TraitFlags |= ReferenceFlag; }
	constexpr void SetVolatileFlag() { TraitFlags |= VolatileFlag; }
	constexpr void SetRValReferenceFlag() { TraitFlags |= RValReferenceFlag; }

	constexpr void RemoveConstFlag() { TraitFlags &= ~ConstFlag; }
	constexpr void RemoveReferenceFlag() { TraitFlags &= ~ReferenceFlag; }
	constexpr void RemoveVolatileFlag() { TraitFlags &= ~VolatileFlag; }
	constexpr void RemoveRValReferenceFlag() { TraitFlags &= ~RValReferenceFlag; }

	constexpr void SetPointerAmount(uint16_t amount) { PointerAmount = amount; }
	constexpr uint32_t GetPointerAmount() const { return PointerAmount; }

	constexpr void SetArraySize(uint32_t Size) { ArraySize = Size; }
	constexpr uint32_t GetArraySize() const { return ArraySize; }

	constexpr bool IsConst() const { return TraitFlags & ConstFlag; }
	constexpr bool IsReference() const { return TraitFlags & ReferenceFlag; }
	constexpr bool IsVolatile() const { return TraitFlags & VolatileFlag; }
	constexpr bool IsRValReference() const { return TraitFlags & RValReferenceFlag; }
	constexpr bool IsPointer() const { return PointerAmount; }
	constexpr bool IsArray() const { return ArraySize == 1; }
	constexpr bool IsRefOrPointer() const { return IsPointer() || IsReference() || IsRValReference(); }

	constexpr uint32_t GetSize() const { return IsRefOrPointer() ? sizeof(void*) : (GetTypeId().GetTypeSize() * GetArraySize()); }
	constexpr uint32_t GetAlign() const { return IsRefOrPointer() ? alignof(void*) : GetTypeId().GetTypeAlignment(); }

	constexpr uint64_t	GetHash() const { return Type.GetId() ^ ArraySize ^ (static_cast<uint64_t>(PointerAmount) << 32) ^ (static_cast<uint64_t>(TraitFlags) << 40); }

	friend bool operator==(const VariableId& lhs, const VariableId& rhs);
private:

	TypeId		Type{ };	// The underlying type id
	uint32_t	ArraySize{ };	// if the variable is a fixed sized array, the size will be contained in this. else it will be 1

	// int* 는 1, int** 는 2
	uint16_t	PointerAmount{ };	// The amount of pointers that are attached to the Type
	uint8_t		TraitFlags{ };	// Other flags (const, volatile, reference, RValReference)
};

template <typename T>
constexpr uint32_t CountPointers(uint32_t counter = 0)
{
	if constexpr (std::is_pointer_v<T>)
		return CountPointers<std::remove_pointer_t<T>>(++counter);
	else
		return counter;
}

// https://github.com/ConnorDeMeyer/Reflection-Tutorial/blob/main/Reflection-Tutorial/Tutorial02-Variables/VariableId.h
template<typename T>
inline constexpr VariableId VariableId::Create()
{
	// 모든 array extent 제거 ex) int[2][3] -> int
	using Type_RemovedExtents = std::remove_all_extents_t<T>;

	// &, && 제거
	using Type_RemovedRefs = std::remove_reference_t<Type_RemovedExtents>;

	// * 제거
	using Type_RemovedPtrs = remove_all_pointers_t<Type_RemovedRefs>;

	// const *, volatile 제거 
	using StrippedType = std::remove_cvref_t<Type_RemovedPtrs>;

	// Type 등록
	RegisterType<StrippedType> TypeRegister{};

	constexpr bool IsRef{ std::is_reference_v<T> };
	constexpr bool IsRValRef{ std::is_rvalue_reference_v<T> };
	constexpr bool IsConst{ std::is_const_v<Type_RemovedPtrs> };
	constexpr bool IsVolatile{ std::is_volatile_v<Type_RemovedPtrs> };

	// 몇개의 pointer 가 있는지 계산한다.
	constexpr uint32_t PointerAmount{ CountPointers<Type_RemovedRefs>() };

	// 원본 type 을 인자로 넘겨서 VariableId 객체를 만든다.
	auto variable = VariableId(TypeId::Create<StrippedType>());

	if constexpr (IsConst)		variable.SetConstFlag();
	if constexpr (IsVolatile)	variable.SetVolatileFlag();
	if constexpr (IsRef)		variable.SetReferenceFlag();
	if constexpr (IsRValRef)	variable.SetRValReferenceFlag();

	variable.SetPointerAmount(PointerAmount);

	// Type_RemovedExtents 가 void 가 "아니라면", 원본 T 는 Array Type 이라는 의미이다.
	// 이 경우 ArraySize 를 계산해줄 것이다. 
	if constexpr (!std::is_same_v<void, Type_RemovedExtents>)
	{
		constexpr uint32_t ArraySize{ sizeof(T) / sizeof(Type_RemovedExtents) };
		variable.SetArraySize(ArraySize);
	}
	else
	{
		variable.SetArraySize(1);
	}

	return variable;
}

// VariableId 가 hash 기반 자료구조의 key 로 사용될 수 있도록 해준다.
// GetVariableName 에서 사용해줄 것이다.
// ex) std::unordered_map or std::unordered_set
template <>
struct std::hash<VariableId>
{
	std::size_t operator()(const VariableId& id) const noexcept
	{
		return static_cast<size_t>(id.GetHash());
	}
};

bool operator==(const VariableId& lhs, const VariableId& rhs)
{
	return	lhs.Type == rhs.Type &&
		lhs.ArraySize == rhs.ArraySize &&
		lhs.PointerAmount == rhs.PointerAmount &&
		lhs.TraitFlags == rhs.TraitFlags;
}

inline const std::string& GetVariableName(const VariableId& variableId)
{
	static std::unordered_map<VariableId, std::string> VariableNames{};

	auto it = VariableNames.find(variableId);

	if (it != VariableNames.end())
	{
		return it->second;
	}

	// 만약 존재하지 않는다면 등록해준다. 
	{
		std::string Name = std::string(variableId.GetTypeId().GetTypeName());

		if (variableId.IsVolatile()) Name = "volatile " + Name;
		if (variableId.IsConst()) Name = "const " + Name;

		const uint32_t pointerAmount = variableId.GetPointerAmount();
		for (uint32_t i{}; i < pointerAmount; ++i)
		{
			Name += '*';
		}

		if (variableId.GetArraySize() > 1)
		{
			Name += '[';
			Name += std::to_string(variableId.GetArraySize());
			Name += ']';
		}

		if (variableId.IsRValReference()) Name += "&&";
		else if (variableId.IsReference()) Name += '&';

		// std::move(Name) : 복사방지
		// first  : 현재 insert한 pair 를 리턴
		// second : value, 이 경우 string 을 리턴
		return VariableNames.emplace(variableId, std::move(Name)).first->second;
	}
}

/*
class GameObject
{
	Transform LocalTransform;	offset 0
	std::string Name;			offset 40
	uint32_t id;				offset 80
}

struct Transform
{
	Vector Translation;  offset 0
	Vector Scale;		 offset 12
	Quaternion Rotation; offset 24
}

struct Vector (Translation)
{
	float x;		offset 0
	float y;		offste 4
	float z;		offset 8
}

struct Vector (Scale)
{
	float x;		offset 12
	float y;		offste 16
	float z;		offset 20
}

struct Quaternion
{
	float x;		offset 24
	float y;		offste 28
	float z;		offset 32
	float w;		offset 36
}
*/