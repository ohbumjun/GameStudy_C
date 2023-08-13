#include "PrecompiledHeader.h"
#include "Serializer.h"

Serializer::Target::Target()
{
}

Serializer::Target::Target(const Reflection::TypeId typeId, void* target) :
	m_ValuePointer(target)
	, m_Type(typeId)
{
}

Serializer::Access::Access(Target&& target, const char* property, int32 index)
	: target(std::move(target))
	, property(property)
	, index(index)
{
}


Serializer::Serializer(SerializeType type)
	: m_Type(type)
{
	registExtensionType();
	m_History.property = nullptr;
	m_History.acceses.clear();
}

Serializer::~Serializer()
{
}

void Serializer::WStartObject()
{
	m_History.acceses.push_back(Access(Target(0, nullptr)));
	wStartObject();
}

void Serializer::WStartObject(const Reflection::TypeId type, void* target)
{
	m_History.acceses.push_back(Access(Target(type, target)));
	wStartObject(type);
}

void Serializer::WKey(const char* key)
{
	m_History.property = key;
	wKey(key);
}

void Serializer::WStartArray(Reflection::TypeId type, uint64 arrayLength)
{
	wStartArray(type, arrayLength);
}

void Serializer::WStartArray(uint64 arrayLength)
{
	wStartArray(arrayLength);
}

void Serializer::RegistWriter(const Reflection::TypeId type, std::function<void(Serializer&, const Reflection::TypeId, void*)> writer)
{
	if (m_Writers.find(type) == m_Writers.end())
	{
		m_Writers.insert(std::make_pair(type, writer));
	}
}

bool Serializer::HasWriter(const Reflection::TypeId type)
{
	return m_Writers.find(type) != m_Writers.end();
}

void Serializer::UnregistWriter(const Reflection::TypeId type)
{
	if (m_Writers.find(type) != m_Writers.end())
	{
		m_Writers.erase(type);
	}
}

void Serializer::RegistReader(const Reflection::TypeId type, std::function<void(Serializer&, const Reflection::TypeId, void*)> reader)
{
	if (m_Readers.find(type) == m_Readers.end())
	{
		m_Readers.insert(std::make_pair(type, reader));
	}
}

bool Serializer::HasReader(const Reflection::TypeId type)
{
	return m_Readers.find(type) != m_Readers.end();

}

void Serializer::UnregistReader(const Reflection::TypeId type)
{
	if (m_Readers.find(type) != m_Readers.end())
	{
		m_Readers.erase(type);
	}
}

void Serializer::registExtensionType()
{
	Reflection::TypeId::Create<Serializer>();
}

void Serializer::Write(const Reflection::TypeId type, void* data)
{

}

void Serializer::Write(const char* key, const Reflection::TypeId type, void* data)
{
}

void Serializer::Write(const char* key, const bool data)
{
}

void Serializer::Write(const char* key, const int8 data)
{
}

void Serializer::Write(const char* key, const uint8 data)
{
}

void Serializer::Write(const char* key, const int16 data)
{
}

void Serializer::Write(const char* key, const uint16 data)
{
}

void Serializer::Write(const char* key, const int32 data)
{
}

void Serializer::Write(const char* key, const uint32 data)
{
}

void Serializer::Write(const char* key, const int64 data)
{
}

void Serializer::Write(const char* key, const uint64 data)
{
}

void Serializer::Write(const char* key, const float data)
{
}

void Serializer::Write(const char* key, const double data)
{
}

void Serializer::Write(const char* key, const std::string& data)
{
}

void Serializer::Write(const char* key, Serializable* data)
{
}

void Serializer::WBuffer(const char* key, void* buffer, size_t size)
{
}

void Serializer::WBuffer(void* buffer, size_t size)
{
}

void Serializer::WEndObject()
{
}
