#include "PrecompiledHeader.h"
#include "JsonDomSerializer.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

JsonDomSerializer::JsonDomSerializer() :
	Serializer(SerializeType::Write)
{
}

JsonDomSerializer::JsonDomSerializer(const char* json) :
	Serializer(SerializeType::Read)
{
	Document* doc = new Document;
	doc = new Document;
	doc->Parse(json);

	if (doc->HasParseError())
	{
		assert(false);
	}

	m_Document = doc;
}

JsonDomSerializer::~JsonDomSerializer()
{
}

std::string JsonDomSerializer::GetResult()
{
	return std::string();
}

void JsonDomSerializer::writeStartObject()
{
}

void JsonDomSerializer::writeKey(const char* key)
{
}

void JsonDomSerializer::write(const bool data)
{
}

void JsonDomSerializer::write(const int8 data)
{
}

void JsonDomSerializer::write(const uint8 data)
{
}

void JsonDomSerializer::write(const int16 data)
{
}

void JsonDomSerializer::write(const uint16 data)
{
}

void JsonDomSerializer::write(const int32 data)
{
}

void JsonDomSerializer::write(const uint32 data)
{
}

void JsonDomSerializer::write(const int64 data)
{
}

void JsonDomSerializer::write(const uint64 data)
{
}

void JsonDomSerializer::write(const float data)
{
}

void JsonDomSerializer::write(const double data)
{
}

void JsonDomSerializer::write(const std::string& data)
{
}

void JsonDomSerializer::write(const char* data)
{
}

void JsonDomSerializer::write(const unsigned char* data)
{
}

void JsonDomSerializer::writeBuffer(void* buffer, size_t size)
{
}

void JsonDomSerializer::writeStartArray(uint64 arrayLength)
{
}

void JsonDomSerializer::writeEndArray()
{
}

void JsonDomSerializer::writeEndObject()
{
}

void JsonDomSerializer::readStartObject()
{
}

void JsonDomSerializer::useKey(const char* key)
{
}

void JsonDomSerializer::readKey(char* key)
{
}

void JsonDomSerializer::read(bool& data)
{
}

void JsonDomSerializer::read(int8& data)
{
}

void JsonDomSerializer::read(uint8& data)
{
}

void JsonDomSerializer::read(int16& data)
{
}

void JsonDomSerializer::read(uint16& data)
{
}

void JsonDomSerializer::read(int32& data)
{
}

void JsonDomSerializer::read(uint32& data)
{
}

void JsonDomSerializer::read(int64& data)
{
}

void JsonDomSerializer::read(uint64& data)
{
}

void JsonDomSerializer::read(float& data)
{
}

void JsonDomSerializer::read(double& data)
{
}

void JsonDomSerializer::read(std::string& data)
{
}

void JsonDomSerializer::read(char* data)
{
}

void JsonDomSerializer::read(unsigned char* data)
{
}

void JsonDomSerializer::readBuffer(void* buffer, size_t size)
{
}

size_t JsonDomSerializer::readStartArray()
{
	return size_t();
}

void JsonDomSerializer::readEndArray()
{
}

void JsonDomSerializer::readEndObject()
{
}
