#pragma once

#include "VariableId.h"
#include <string>
#include <set>

class MemberId final
{
public:

	struct MemberInfo
	{
		std::string Name{ };
		Reflection::VariableId	VariableId{ };
		uint32_t	Offset{ };
		uint32_t	Size{ };
		uint32_t	Align{ };

		constexpr bool operator<(const MemberInfo& rhs) const
		{
			return Offset < rhs.Offset;
		}
	};

public:

	constexpr MemberId(Reflection::TypeId id, uint32_t Offset) : Id{ id }, Offset{ Offset } { };

	constexpr MemberId() = default;
	// constexpr ~MemberId() = default;
	~MemberId() = default;
	constexpr MemberId(const MemberId&) = default;
	constexpr MemberId& operator=(const MemberId&) = default;
	constexpr MemberId(MemberId&&) noexcept = default;
	constexpr MemberId& operator=(MemberId&&) noexcept = default;

public:

	constexpr Reflection::TypeId	GetTypeId()	const { return Id; }
	constexpr uint32_t	GetOffset()	const { return Offset; }

	const MemberInfo& GetMemberInfo()	const;
	const std::string& GetName()	const { return GetMemberInfo().Name; }
	Reflection::VariableId			GetVariableId()	const { return GetMemberInfo().VariableId; }
	uint32_t			GetSize()	const { return GetMemberInfo().Size; }
	uint32_t			GetAlignment()	const { return GetMemberInfo().Align; }

public:

	template <typename Class, typename Field>
	static MemberId RegisterField(const std::string& fieldName, uint32_t Offset);

	template <typename Field>
	static MemberId RegisterField(Reflection::TypeId classId, const std::string& fieldName, uint32_t Offset);

	static MemberId RegisterField(Reflection::TypeId classId, Reflection::VariableId MemberId, const std::string& fieldName, uint32_t Offset, uint32_t Size, uint32_t Align);

	static const std::set<MemberInfo>& GetMemberInfos(Reflection::TypeId id) { return GetStatics().MemberInfos[id]; }

	static const MemberInfo& GetMemberInfo(Reflection::TypeId id, const std::string& FieldName);

	static const auto& GetAllMemberInfos() { return GetStatics().MemberInfos; }

	static bool Exists(Reflection::TypeId classId) { return GetStatics().MemberInfos.find(classId) != GetStatics().MemberInfos.end(); }

private:

	struct StaticData
	{
		// std::set -> offset 에 따라 sort 될 것이다.
		std::unordered_map<Reflection::TypeId, std::set<MemberInfo>> MemberInfos{};

		// Type -> (field 이름, offset)
		std::unordered_map<Reflection::TypeId, std::unordered_map<std::string, uint32_t>> MemberInfoNameMap{};
	};

	static StaticData& GetStatics()
	{
		static StaticData data{};
		return data;
	}


private:

	Reflection::TypeId		Id{ };
	uint32_t	Offset{ };
};


struct RegisterMember final
{
	RegisterMember(Reflection::TypeId classId, Reflection::VariableId MemberId, const std::string& fieldName, uint32_t Offset, uint32_t Size, uint32_t Align)
	{
		MemberId::RegisterField(classId, MemberId, fieldName, Offset, Size, Align);
	}
};

#define REGISTER_MEMBER(TYPE, FIELD) inline static RegisterMember TYPE##FIELD{TypeId::Create<TYPE>(), VariableId::Create<decltype(TYPE::FIELD)>(), #FIELD, offsetof(TYPE, FIELD), sizeof(decltype(TYPE::FIELD)), alignof(decltype(TYPE::FIELD))};

inline const MemberId::MemberInfo& MemberId::GetMemberInfo() const
{
	// 해당 Type 의 멤버정보들을 리턴
	auto& MemberInfos = GetMemberInfos(Id);

	MemberInfo InfoToFind{};
	InfoToFind.Offset = Offset;

	// 해당 id Type의 멤버들 중에서 Offset 값이 현재 Offset 값과 동일한 MemberInfo 객체 정보를 리턴한다. 
	auto it = MemberInfos.find(InfoToFind);

	// should always contain the value, else something went wrong
	assert(it != MemberInfos.end());

	return *it;
}

inline const MemberId::MemberInfo& MemberId::GetMemberInfo(Reflection::TypeId id, const std::string& FieldName)
{
	// 현재 Type 의 모든 (Field, Offset) 정보를 리턴
	auto& MemberInfoNames = GetStatics().MemberInfoNameMap[id];

	assert(MemberInfoNames.find(FieldName) != MemberInfoNames.end());

	// 현재 Type, 찾고자하는 Field 의 Offset -> MemberID 구성
	const MemberId MemberId{ id, MemberInfoNames[FieldName] };

	return MemberId.GetMemberInfo();
}


inline MemberId MemberId::RegisterField(Reflection::TypeId classId, Reflection::VariableId VariableID, const std::string& fieldName, uint32_t Offset, uint32_t Size, uint32_t Align)
{
	MemberInfo info{};
	info.Name = fieldName;
	info.VariableId = VariableID;
	info.Offset = Offset;
	info.Size = Size;
	info.Align = Align;

	GetStatics().MemberInfos[classId].emplace(info);
	GetStatics().MemberInfoNameMap[classId].emplace(fieldName, Offset);

	return { classId, Offset };
}

template<typename Class, typename Field>
inline MemberId MemberId::RegisterField(const std::string& fieldName, uint32_t Offset)
{
	// Type 관리자에 Class, Field 정보를 모두 추가한다. 
	auto registerClass = RegisterType<Class>();
	auto registerField = RegisterType<Field>();

	return RegisterField(
		TypeId::Create<Class>(),
		VariableId::Create<Field>(),
		fieldName,
		Offset,
		sizeof(Field),
		alignof(Field));
}

template<typename Field>
inline MemberId MemberId::RegisterField(Reflection::TypeId classId, const std::string& fieldName, uint32_t Offset)
{
	auto registerField = RegisterType<Field>();

	return RegisterField(
		classId,
		VariableId::Create<Field>(),
		fieldName,
		Offset,
		sizeof(Field),
		alignof(Field));
}
