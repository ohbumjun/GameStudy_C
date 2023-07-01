#pragma once
#include "TypeInfo.h"
#include "Serializable.h"

enum class SerializeType
{
	Read,
	Write
};

class Serializer
{
public :
	enum class UsingType : uint8
	{
		NONE = 0,
		WRITE,
		READ
	};

	Serializer(SerializeType type);
	Serializer() = default;
	virtual ~Serializer();


	void WStartObject();
	void WStartObject(const TypeId type, void* target = nullptr);
	template<typename T>
	void WStartObject(T* target) { WriteStartObject(Reflection::GetTypeId<T>(), target); }
	void WKey(const char* key);
	void WStartArray(uint64 arrayLength);
	void WStartArray(TypeId type, uint64 arrayLength);
	void WKey(const char* key);
	template<typename T>
	void WStartArray(uint64 arrayLength) { WriteStartArray(Reflection::GetTypeId<T>(), arrayLength); }
	void WStartArray(TypeId type, uint64 arrayLength);
	inline void WEndArray();
	void WEndArray();

	template<typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
	void Write(const T& data)
	{
		write(data);
	}

	template<typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
	void Write(T& data)
	{
		write(data);
	}

	void Write(const TypeId type, void* data);

	/*
	   @brief for Property property->GetValue<T>() Serialize compile error fix
	 - TODO void* Serialize way fix
	*/
	void Write(const char* key, void* data) {}
	void Write(const char* key, const TypeId type, void* data);

	void Write(const char* key, const bool data);
	void Write(const char* key, const int8 data);
	void Write(const char* key, const uint8 data);
	void Write(const char* key, const int16 data);
	void Write(const char* key, const uint16 data);
	void Write(const char* key, const int32 data);
	void Write(const char* key, const uint32 data);
	void Write(const char* key, const int64 data);
	void Write(const char* key, const uint64 data);
#ifdef __MACOSX__
	void Write(const char* key, const size_t& data/*uint64*/);
	void Write(const char* key, const time_t& data/*int64*/);
#endif
	void Write(const char* key, const float data);
	void Write(const char* key, const double data);
	void Write(const char* key, const std::string& data);
	// void Write(const char* key, const LvVec4f& data);
	// void Write(const char* key, const LvQuatf& data);
	// void Write(const char* key, const LvVec3f& data);
	// void Write(const char* key, const LvVec2f& data);
	// void Write(const char* key, const LvMat3f& data);
	// void Write(const char* key, const LvMat4f& data);
	// void Write(const char* key, const LvColor& data);
	// void Write(const char* key, const LvEntity& data);
	void Write(const char* key, Serializable& data);
	void WBuffer(const char* key, void* buffer, size_t size);
	void WBuffer(void* buffer, size_t size);
	void WEndObject();

	template<typename T>
	void Write(const char* key, std::list<T>& t)
	{
		static LvTypeId type = LvReflection::GetTypeId<T>();
		static bool registed = LvReflection::HasRegist(type);
		if (false == registed) LvReflection::Regist<T>();

		writeKey(key);
		writeStartArray(type, t.Count());
		if (0 < t.Count())
		{
			for (size_t i = 0; i < t.Count(); ++i)
			{
				write((T&)t[i]);
			}
		}
		writeEndArray();
	}

protected:
	virtual void wStartObject() = 0;
	virtual void wStartObject(TypeId type) = 0;
	virtual void wKey(const char* key) = 0;
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
	virtual void write(const char* data) {}
	virtual void write(const unsigned char* data) {}
	// virtual void write(const LvDynamicObject& object) = 0;
	virtual void wBuffer(void* buffer, size_t size) = 0;
	virtual void wStartArray(uint64 arrayLength) = 0;
	// virtual void writeStartArray(LvType type, uint64 arrayLength) = 0;
	virtual void wEndArray() = 0;
	virtual void wEndObject() = 0;

	/*read*/
	virtual void rStartObject() = 0;
	virtual void rStartObject(TypeId type) = 0;
	virtual void useKey(const char* key) = 0;
	virtual bool hasKey(const char* key) { return false; };
	virtual void rKey(char* key) = 0;
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
	virtual void read(char* data) {}
	virtual void read(unsigned char* data) {}
	// virtual void read(LvDynamicObject& object) = 0;
	virtual void rBuffer(void* buffer, size_t size) = 0;
	virtual size_t rStartArray() = 0;
	// size_t readStartArray(LvType type) = 0;
	virtual void rEndArray() = 0;
	virtual void rEndObject() = 0;

	struct Target
	{
		const TypeId type = 0;

		void* pointer = nullptr;

		Target();

		Target(const TypeId typeId, void* target);
	};

	struct Access
	{
		const char* property = nullptr;

		int32 index = -1;

		Target target;

		Access(Target&& target, const char* property = nullptr, int32 index = -1);
	};

	struct Context
	{
		const char* property = nullptr;

		std::list<Access> acceses;

	} _context;

	// virtual void readObject(LvDynamicObject& object, virtual void* value)= 0;
	// virtual void readArray(LvDynamicObject& object, virtual void* value)= 0;

	/**
	* @brief type 을 serialize 하기 위한 writer 함수 등록
	*/
	static void RegistWriter(const TypeId type, std::function<void(Serializer&, const TypeId, void*)> writer);

	/**
	* @brief type 에 Writer 가 등록되어 있는지 확인
	*/
	static bool HasWriter(const TypeId type);

	/**
	* @brief type 에 Writer 를 제거
	*/
	static void UnregistWriter(const TypeId type);

	/**
	* @brief type 을 deserialize 하기 위한 Reader 함수 등록
	*/
	static void RegistReader(const TypeId type, std::function<void(Serializer&, const TypeId, void*)> reader);

	/**
	* @brief type 에 Reader 가 등록되어 있는지 확인
	*/
	static bool HasReader(const TypeId type);

	/**
	* @brief type 에 Reader 를 제거
	*/
	static void UnregistReader(const TypeId type);

protected:

	static void registExtensionType();


private :
	SerializeType m_Type;

	static std::set<TypeId> _defaultExtensionTypes;
	static std::unordered_map<TypeId, std::function<void(Serializer&, const TypeId, void*)>> _writers;
	static std::unordered_map<TypeId, std::function<void(Serializer&, const TypeId, void*)>> _readers;
};

