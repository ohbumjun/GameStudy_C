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

std::string JsonDomSerializer::GetFinalResult()
{
	return m_JsonArchive.GetFinalResult();
}

void JsonDomSerializer::wStartObject()
{
	m_JsonArchive.wStartObject();
}

void JsonDomSerializer::wStartObject(Reflection::TypeId type)
{
	m_JsonArchive.wStartObject();
}

void JsonDomSerializer::wKey(const char* key)
{
	m_JsonArchive.wKey(key);
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

void JsonDomSerializer::wBuffer(void* buffer, size_t size)
{
	m_JsonArchive.wBuffer(buffer, size);
}

void JsonDomSerializer::wStartArray(uint64 arrayLength)
{
	m_JsonArchive.wStartArray(arrayLength);
}

void JsonDomSerializer::wStartArray(Reflection::TypeId type, uint64 arrayLength)
{
	m_JsonArchive.WStartArray(type, arrayLength);
}

void JsonDomSerializer::wEndArray()
{
	m_JsonArchive.wEndArray();
}

void JsonDomSerializer::wEndObject()
{
	m_JsonArchive.wEndObject();
}

void JsonDomSerializer::rStartObject()
{
	Value* v = nullptr;

	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		Value* val = static_cast<Value*>(getNextValue(History));

		if (nullptr != val && val->IsObject())
		{
			//���� ��ȿ�ϴٸ� set
			v = val;
		}
	}
	else
	{
		//����! document push
		v = static_cast<Document*>(m_Document);
	}

	m_ReadHistoryStack.push(History(v));
}

void JsonDomSerializer::rStartObject(Reflection::TypeId type)
{
	rStartObject();
}

void JsonDomSerializer::useKey(const char* key)
{
	if (m_ReadHistoryStack.size() == 0) return;

	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ��� Value�� nullptr��� keyStack���� ó���� �� ����
		History* History = &m_ReadHistoryStack.top();

		if (nullptr != History->value)
		{
			m_KeyStack.push(key);
		}
	}
}

bool JsonDomSerializer::hasKey(const char* key)
{
	if (m_ReadHistoryStack.size() == 0) return false;

	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ��� Value�� nullptr��� keyStack���� ó���� �� ����
		History* History = &m_ReadHistoryStack.top();

		if (nullptr != History->value)
		{
			return static_cast<Value*>(History->value)->HasMember(key);
		}
	}

	return false;
}

void JsonDomSerializer::rKey(char* key)
{

}

void JsonDomSerializer::read(bool& data)
{
	rapidjson::Value* val = nullptr;
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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

	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));
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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();
		rapidjson::Value* v = nullptr;

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

	}
	else
	{
		val = static_cast<Document*>(m_Document);
	}

	//set Data
	if (nullptr != val && val->IsFloat())
	{
		data = val->GetFloat();
	}
}

void JsonDomSerializer::read(double& data)
{
	rapidjson::Value* val = nullptr;
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));

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
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		val = static_cast<rapidjson::Value*>(getNextValue(History));
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

void JsonDomSerializer::rBuffer(void* buffer, size_t size)
{
	char* dest = static_cast<char*>(buffer);
	int originSize = static_cast<int>(size);
	char* result = nullptr;

	// converting binary data into a string of ASCII characters using the Base64 encoding scheme.
	// int encodeSize = lv_base64_encode_size(originSize);
	int encodeSize = base64_encode(dest, originSize, &result);

	std::string encodeData;
	read(encodeData);

	if (false == encodeData.empty())//data�� �ִٸ� ���۸� ����
	{
		// lv_base64_decode(dest, encodeData.c_str(), encodeSize);
		base64_decode(const_cast<char*>(encodeData.c_str()), encodeSize, dest);
	}
}

size_t JsonDomSerializer::rStartArray()
{
	size_t size = 0;
	rapidjson::Value* v = nullptr;
	if (m_ReadHistoryStack.size() > 0)
	{
		//�θ� ����
		History* History = &m_ReadHistoryStack.top();

		//get next value
		rapidjson::Value* val = static_cast<rapidjson::Value*>(getNextValue(History));

		if (nullptr != val && val->IsArray())
		{
			//���� ��ȿ�ϴٸ� set
			v = val;
			size = v->Size();
		}
	}
	else
	{
		//����! document push
		v = static_cast<Document*>(m_Document);
	}

	m_ReadHistoryStack.push(History(v));
	return size;
}

size_t JsonDomSerializer::rStartArray(Reflection::TypeId type)
{
	return rStartArray();
}

void JsonDomSerializer::rEndArray()
{
	if (m_ReadHistoryStack.size() <= 0)
	{
		assert(false);
		// LV_LOG(warning, "LvJsonDomArchive::readEndArray warnning : read container overflow");
		return;
	}
	m_ReadHistoryStack.pop();
}

void JsonDomSerializer::rEndObject()
{
	if (m_ReadHistoryStack.size() <= 0)
	{
		// LV_LOG(warning, "LvJsonDomArchive::readEndObject warnning : read container overflow");
		assert(false);
		return;
	}

	m_ReadHistoryStack.pop();
}

void* JsonDomSerializer::getNextValue(History* History)
{
	Value* v = nullptr;
	Value* HistoryValue = static_cast<Value*>(History->value);

	// Array Type
	if (nullptr != HistoryValue && HistoryValue->IsArray())
	{
		// Array �󿡼� Ư�� index �� �ִ� ���� �����´�.
		v = HistoryValue->Size() <= History->elementNum ? nullptr : &HistoryValue->operator[](
			static_cast<SizeType>(History->elementNum++));

		if (v == nullptr || (v != nullptr && v->IsNull()))
		{
			assert(false);
		}
	}

	// Object Type
	else if (nullptr != History->value && HistoryValue->IsObject())
	{
		// Object �� �����Ǵ� Key ���� �����´�.
		const char* key = m_KeyStack.top();
		m_KeyStack.pop();
		if (HistoryValue->HasMember(key))
		{
			v = &HistoryValue->operator[](Value(key, static_cast<SizeType>(strlen(key))));
		}

		if (v == nullptr || (v != nullptr && v->IsNull()))
		{
			assert(false);
		}
	}

	return v;
}
