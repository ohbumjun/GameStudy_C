#pragma once

enum class SerializeType
{
	Read,
	Write
};

class Serializer
{
public :
	Serializer(SerializeType type);
protected:
	virtual void writeStartObject() = 0;
	// virtual void writeStartObject(LvType type) = 0;
	virtual void writeKey(const char* key) = 0;
	virtual void write(const bool data) = 0;
	virtual void write(const int8 data) = 0;
	virtual void write(const uint8 data) = 0;
	virtual void write(const int16 data) = 0;
	virtual void write(const uint16 data) = 0;
	virtual void write(const int32 data) = 0;
	virtual void write(const uint32 data) = 0;
	virtual void write(const int64 data) = 0;
	virtual void write(const uint64 data) = 0;
	virtual void write(const float data) = 0;
	virtual void write(const double data) = 0;
	virtual void write(const std::string& data) = 0;
	virtual void write(const char* data) = 0;
	virtual void write(const unsigned char* data) = 0;
	// virtual void write(const LvDynamicObject& object) = 0;
	virtual void writeBuffer(void* buffer, size_t size) = 0;
	virtual void writeStartArray(uint64 arrayLength) = 0;
	// virtual void writeStartArray(LvType type, uint64 arrayLength) = 0;
	virtual void writeEndArray() = 0;
	virtual void writeEndObject() = 0;

protected:
	virtual void readStartObject() = 0;
	// virtual void readStartObject(LvType type) = 0;
	virtual void useKey(const char* key) = 0;
	bool hasKey(const char* key) { return false; };
	virtual void readKey(char* key) = 0;
	virtual void read(bool& data) = 0;
	virtual void read(int8& data) = 0;
	virtual void read(uint8& data) = 0;
	virtual void read(int16& data) = 0;
	virtual void read(uint16& data) = 0;
	virtual void read(int32& data) = 0;
	virtual void read(uint32& data) = 0;
	virtual void read(int64& data) = 0;
	virtual void read(uint64& data) = 0;
	virtual void read(float& data) = 0;
	virtual void read(double& data) = 0;
	virtual void read(std::string& data) = 0;
	virtual void read(char* data) = 0;
	virtual void read(unsigned char* data) = 0;
	// virtual void read(LvDynamicObject& object) = 0;
	virtual void readBuffer(void* buffer, size_t size) = 0;
	virtual size_t readStartArray() = 0;
	// size_t readStartArray(LvType type) = 0;
	virtual void readEndArray() = 0;
	virtual void readEndObject() = 0;

	// virtual void readObject(LvDynamicObject& object, virtual void* value)= 0;
	// virtual void readArray(LvDynamicObject& object, virtual void* value)= 0;

private :
	SerializeType m_Type;
};

