#include "PrecompiledHeader.h"
#include "JsonDomSerializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

// custom class
#include "Encode.h"

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
	Document* doc = reinterpret_cast<Document*>(m_Document);

	if (doc)
	{
		delete doc;
	}
}

std::string JsonDomSerializer::GetResult()
{
	return m_JsonArchive.GetResult();
}

void JsonDomSerializer::writeStartObject()
{
	m_JsonArchive.writeStartObject();
}

void JsonDomSerializer::writeStartObject(TypeId type)
{
}

void JsonDomSerializer::writeKey(const char* key)
{
	m_JsonArchive.writeKey(key);
}

void JsonDomSerializer::write(const bool data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const int8 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const uint8 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const int16 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const uint16 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const int32 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const uint32 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const int64 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const uint64 data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const float data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const double data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const std::string& data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const char* data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::write(const unsigned char* data)
{
	m_JsonArchive.write(data);
}

void JsonDomSerializer::writeBuffer(void* buffer, size_t size)
{
	m_JsonArchive.writeBuffer(buffer, size);
}

void JsonDomSerializer::writeStartArray(uint64 arrayLength)
{
	m_JsonArchive.writeStartArray(arrayLength);
}

void JsonDomSerializer::writeEndArray()
{
	m_JsonArchive.writeEndArray();
}

void JsonDomSerializer::writeEndObject()
{
	m_JsonArchive.writeEndObject();
}

void JsonDomSerializer::readStartObject()
{
	Value* v = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻음
		Context* context = &_contextStack.top();

		//get next value
		Value* val = static_cast<Value*>(getNextValue(context));

		if (nullptr != val && val->IsObject())
		{
			//값이 유효하다면 set
			v = val;
		}
	}
	else
	{
		//시작! document push
		v = static_cast<Document*>(m_Document);
	}

	_contextStack.push(Context(v));
}

void JsonDomSerializer::readStartObject(TypeId type)
{
}

void JsonDomSerializer::useKey(const char* key)
{
	if (_contextStack.size() == 0) return;

	if (_contextStack.size() > 0)
	{
		//부모의 Value가 nullptr라면 keyStack관련 처리를 다 무시
		Context* context = &_contextStack.top();
		if (nullptr != context->value)
		{
			_keyStack.push(key);
		}
	}
}

bool JsonDomSerializer::hasKey(const char* key)
{
	if (_contextStack.size() == 0) return false;

	if (_contextStack.size() > 0)
	{
		//부모의 Value가 nullptr라면 keyStack관련 처리를 다 무시
		Context* context = &_contextStack.top();

		if (nullptr != context->value)
		{
			return static_cast<Value*>(context->value)->HasMember(key);
		}
	}

	return false;
}

void JsonDomSerializer::readKey(char* key)
{

}

void JsonDomSerializer::read(bool& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsBool())
	{
		data = val->GetBool();
	}
}

void JsonDomSerializer::read(int8& data)
{
	rapidjson::Value* val = nullptr;

	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));
	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsInt())
	{
		data = val->GetInt();
	}
}

void JsonDomSerializer::read(uint8& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsUint())
	{
		data = val->GetUint();
	}
}

void JsonDomSerializer::read(int16& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsInt())
	{
		data = val->GetInt();
	}
}

void JsonDomSerializer::read(uint16& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsUint())
	{
		data = val->GetUint();
	}
}

void JsonDomSerializer::read(int32& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsInt())
	{
		data = val->GetInt();
	}
}

void JsonDomSerializer::read(uint32& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsUint())
	{
		data = val->GetUint();
	}
}

void JsonDomSerializer::read(int64& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsInt64())
	{
		data = val->GetInt();
	}
}

void JsonDomSerializer::read(uint64& data)
{
}

void JsonDomSerializer::read(float& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsUint64())
	{
		data = val->GetUint64();
	}
}

void JsonDomSerializer::read(double& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsDouble())
	{
		data = val->GetDouble();
	}
}

void JsonDomSerializer::read(std::string& data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsString())
	{
		data = val->GetString();
	}
}

void JsonDomSerializer::read(char* data)
{
	rapidjson::Value* val = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옮
		Context* context = &_contextStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(context));
	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsString())
	{
		const char* str = val->GetString();
		strcpy(data, str);

	}
}

void JsonDomSerializer::read(unsigned char* data)
{
}

void JsonDomSerializer::readBuffer(void* buffer, size_t size)
{
	char* dest = static_cast<char*>(buffer);
	int originSize = static_cast<int>(size);
	char* result = nullptr;

	// converting binary data into a string of ASCII characters using the Base64 encoding scheme.
	// int encodeSize = lv_base64_encode_size(originSize);
	int encodeSize = base64_encode(dest, originSize, &result);

	std::string encodeData;
	read(encodeData);

	if (false == encodeData.empty())//data가 있다면 버퍼를 읽음
	{
		// lv_base64_decode(dest, encodeData.c_str(), encodeSize);
		base64_decode(const_cast<char*>(encodeData.c_str()), encodeSize, dest);
	}
}

size_t JsonDomSerializer::readStartArray()
{
	size_t size = 0;
	rapidjson::Value* v = nullptr;
	if (_contextStack.size() > 0)
	{
		//부모 얻어옴
		Context* context = &_contextStack.top();

		//get next value
		rapidjson::Value* val = static_cast<rapidjson::Value*>(getNextValue(context));

		if (nullptr != val && val->IsArray())
		{
			//값이 유효하다면 set
			v = val;
			size = v->Size();
		}
	}
	else
	{
		//시작! document push
		v = static_cast<Document*>(m_Document);
	}

	_contextStack.push(Context(v));
	return size;
}

void JsonDomSerializer::readEndArray()
{
	if (_contextStack.size() <= 0)
	{
		assert(false);
		// LV_LOG(warning, "LvJsonDomArchive::readEndArray warnning : read container overflow");
		return;
	}
	_contextStack.pop();
}

void JsonDomSerializer::readEndObject()
{
	if (_contextStack.size() <= 0)
	{
		// LV_LOG(warning, "LvJsonDomArchive::readEndObject warnning : read container overflow");
		assert(false);
		return;
	}

	_contextStack.pop();
}

void* JsonDomSerializer::getNextValue(Context* context)
{
	Value* v = nullptr;
	Value* contextValue = static_cast<Value*>(context->value);
	//get rapidjson Value
	if (nullptr != contextValue && contextValue->IsArray())
	{
		v = contextValue->Size() <= context->objNum ? nullptr : &contextValue->operator[](static_cast<SizeType>(context->objNum++));

		if (v == nullptr || (v != nullptr && v->IsNull()))
		{
			// LV_LOG(warning, "LvJsonDomArchive::getNextValue warnning : %zd array property is null", context->objNum);
			assert(false);
		}
	}
	else if (nullptr != context->value && contextValue->IsObject())
	{
		const char* key = _keyStack.top();
		_keyStack.pop();
		if (contextValue->HasMember(key))
		{
			v = &contextValue->operator[](Value(key, static_cast<SizeType>(strlen(key))));
		}

		if (v == nullptr || (v != nullptr && v->IsNull()))
		{
			// LV_LOG(warning, "LvJsonDomArchive::getNextValue warnning : %s object property is null", key);
			assert(false);
		}
	}

	return v;
}
