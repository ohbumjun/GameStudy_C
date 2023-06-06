#pragma once

#include "Serializer.h"

class JsonDomSerializer : public Serializer
{
public :
	JsonDomSerializer();
	JsonDomSerializer(const char* json);
	virtual ~JsonDomSerializer();

	std::string GetResult();

private:
	virtual void writeStartObject() ;
	// virtual void writeStartObject(LvType type) ;
	virtual void writeKey(const char* key) ;
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
	virtual void writeBuffer(void* buffer, size_t size) ;
	virtual void writeStartArray(uint64 arrayLength) ;
	// virtual void writeStartArray(LvType type, uint64 arrayLength) ;
	virtual void writeEndArray() ;
	virtual void writeEndObject() ;

private:
	virtual void readStartObject() ;
	// virtual void readStartObject(LvType type) ;
	virtual void useKey(const char* key) ;
	bool hasKey(const char* key) { return false; };
	virtual void readKey(char* key) ;
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
	virtual void readBuffer(void* buffer, size_t size) ;
	virtual size_t readStartArray() ;
	// size_t readStartArray(LvType type) ;
	virtual void readEndArray() ;
	virtual void readEndObject() ;


private :
	struct Context
	{
		void* value = nullptr;
		size_t objNum = 0;

		Context() = default;
		Context(void* value) : value(){}
	};

	void* getNextValue(Context* context);

	void* m_Document;
	std::stack<Context> _contextStack;
	std::stack<const char*> _keyStack;
};

