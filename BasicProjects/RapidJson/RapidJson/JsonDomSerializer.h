#pragma once

#include "Serializer.h"
#include "JsonSerializer.h"

class JsonDomSerializer : public Serializer
{
public :
	JsonDomSerializer();
	JsonDomSerializer(const char* json);
	virtual ~JsonDomSerializer();

	std::string GetFinalResult();

private:
	virtual void wStartObject() ;
	virtual void wStartObject(Reflection::TypeId type);
	virtual void wKey(const char* key) ;
	virtual void write(const bool data) ;
	virtual void write(const int8 data) ;
	virtual void write(const uint8 data) ;
	virtual void write(const int16 data) ;
	virtual void write(const uint16 data) ;
	virtual void write(const int32 data) ;
	virtual void write(const uint32 data) ;
	virtual void write(const int64 data) ;
	virtual void write(const uint64 data) ;
	virtual void write(const float data) ;
	virtual void write(const double data) ;
	virtual void write(const std::string& data) ;
	virtual void write(const char* data) ;
	virtual void write(const unsigned char* data) ;
	// virtual void write(const LvDynamicObject& object) ;
	virtual void wBuffer(void* buffer, size_t size) ;
	virtual void wStartArray(uint64 arrayLength) ;
	virtual void wStartArray(Reflection::TypeId type, uint64 arrayLength) ;
	virtual void wEndArray() ;
	virtual void wEndObject() ;

private:
	virtual void rStartObject() ;
	virtual void rStartObject(Reflection::TypeId type) ;
	virtual void useKey(const char* key) ;
	virtual bool hasKey(const char* key);
	virtual void rKey(char* key) ;
	virtual void read(bool& data) ;
	virtual void read(int8& data) ;
	virtual void read(uint8& data) ;
	virtual void read(int16& data) ;
	virtual void read(uint16& data) ;
	virtual void read(int32& data) ;
	virtual void read(uint32& data) ;
	virtual void read(int64& data) ;
	virtual void read(uint64& data) ;
	virtual void read(float& data) ;
	virtual void read(double& data) ;
	virtual void read(std::string& data) ;
	virtual void read(char* data) ;
	virtual void read(unsigned char* data) ;
	// virtual void read(LvDynamicObject& object) ;
	virtual void rBuffer(void* buffer, size_t size) ;
	virtual size_t rStartArray() ;
	virtual size_t rStartArray(Reflection::TypeId type) ;
	virtual void rEndArray() ;
	virtual void rEndObject() ;


private :
	struct History
	{
		void* value = nullptr;
		size_t elementNum = 0;   // ?

		History() = default;
		History(void* value) : value(){}
	};

	void* getNextValue(History* context);

	void* m_Document;
	std::stack<History> m_ReadHistoryStack;

	// ex) 하나의 History 내에 여러 Key 가 있을 수 있다.
	// ex) {"a" : 1, "b" : 2}
	std::stack<const char*> m_KeyStack;

	// JsonSerialize 는 Write 시에만 사용한다.
	JsonSerializer m_JsonArchive;
};

