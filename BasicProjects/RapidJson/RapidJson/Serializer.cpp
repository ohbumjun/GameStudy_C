#include "PrecompiledHeader.h"
#include "Serializer.h"


Serializer::Target::Target()
{
}

Serializer::Target::Target(const TypeId typeId, void* target) :
	pointer(target)
	, type(typeId)
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
	_context.property = nullptr;
	_context.acceses.clear();
}

Serializer::~Serializer()
{
}

void Serializer::WStartObject()
{
	_context.acceses.push_back(Access(Target(0, nullptr)));
	wStartObject();
}

void Serializer::WStartObject(const TypeId type, void* target)
{
	_context.acceses.push_back(Access(Target(type, target)));
	wStartObject(type);
}


void Serializer::RegistWriter(const TypeId type, std::function<void(Serializer&, const TypeId, void*)> writer)
{
	if (_writers.find(type) == _writers.end())
	{
		_writers.insert(std::make_pair(type, writer));
	}
}

bool Serializer::HasWriter(const TypeId type)
{
	return _writers.find(type) != _writers.end();
}

void Serializer::UnregistWriter(const TypeId type)
{
	if (_writers.find(type) != _writers.end())
	{
		_writers.erase(type);
	}
}

void Serializer::RegistReader(const TypeId type, std::function<void(Serializer&, const TypeId, void*)> reader)
{
	if (_readers.find(type) == _readers.end())
	{
		_readers.insert(std::make_pair(type, reader));
	}
}

bool Serializer::HasReader(const TypeId type)
{
	return _readers.find(type) != _readers.end();

}

void Serializer::UnregistReader(const TypeId type)
{
	if (_readers.find(type) != _readers.end())
	{
		_readers.erase(type);
	}
}

void Serializer::registExtensionType()
{
	TypeId::Create<Serializer>();
}

void Serializer::Write(const TypeId type, void* data)
{
}

void Serializer::Write(const char* key, const TypeId type, void* data)
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

void Serializer::Write(const char* key, Serializable& data)
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
