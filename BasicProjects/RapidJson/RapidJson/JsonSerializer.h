#pragma once

#include "Serializer.h"

struct ReadHandler;

class JsonSerializer : public Serializer
{
	friend class JsonDomSerializer;
public:
	JsonSerializer();

	JsonSerializer(const char* json);

	virtual ~JsonSerializer();

	std::string GetResult();

private:
	void wStartObject() override;
	void wStartObject(TypeId type) override;
	void wKey(const char* key) override;
	void write(const bool data) override;
	void write(const int8 data) override;
	void write(const uint8 data) override;
	void write(const int16 data) override;
	void write(const uint16 data) override;
	void write(const int32 data) override;
	void write(const uint32 data) override;
	void write(const int64 data) override;
	void write(const uint64 data) override;
	void write(const float data) override;
	void write(const double data) override;
	void write(const std::string& data) override;
	void wBuffer(void* buffer, size_t size) override;
	void wStartArray(uint64 arrayLength) override;
	// void writeStartArray(LvTypeId type, uint64 arrayLength) override;
	void wEndArray() override;
	void wEndObject() override;

	void* _sbWriter = nullptr;
	void* _writer = nullptr;
	int _keyCounter = 0;

private:

	void rStartObject() override;
	void rStartObject(TypeId type) override;
	void useKey(const char* key) override;
	bool hasKey(const char* key) override;
	void rKey(char* key) override;
	void read(bool& data) override;
	void read(int8& data) override;
	void read(uint8& data) override;
	void read(int16& data) override;
	void read(uint16& data) override;
	void read(int32& data) override;
	void read(uint32& data) override;
	void read(int64& data) override;
	void read(uint64& data) override;
	void read(float& data) override;
	void read(double& data) override;
	void read(std::string& data) override;
	// void read(LvDocumentObject& object) override;
	void rBuffer(void* buffer, size_t size) override;
	size_t rStartArray() override;
	// size_t readStartArray(LvTypeId type) override;
	void rEndArray() override;
	void rEndObject() override;

private:

	ReadHandler* _readHandler = nullptr;
};

