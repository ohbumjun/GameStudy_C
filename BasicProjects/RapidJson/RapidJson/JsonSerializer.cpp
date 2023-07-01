#include "PrecompiledHeader.h"
#include "JsonSerializer.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>

// custom class
#include "Encode.h"

using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

struct  JsonReader
{
    JsonReader() { }

    JsonReader(const JsonReader& o)
        : _tokens(o._tokens)
    {
    }

    ~JsonReader()
    {
    }

    bool ReadBool()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // assert(t.type == TokenType::BOOL, "Not matched type corrected type is [BOOL]");
        return t.variant.boolValue;
    }

    int ReadInt()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::INT || t.type == TokenType::UINT, "Not matched type corrected type is [INT]");
        return static_cast<int>(t.variant.intValue);
    }

    int64 ReadInt64()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::INT || t.type == TokenType::UINT, "Not matched type corrected type is [INT]");

        return t.variant.intValue;
    }

    uint32 ReadUint()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::UINT, "Not matched type corrected type is [UINT]");

        return static_cast<uint32>(t.variant.uintValue);
    }

    uint64 ReadUint64()
    {
        const Token& t = _tokens.front();

        // LV_CHECK(t.type == TokenType::UINT, "Not matched type corrected type is [UINT]");

        return t.variant.uintValue;
    }

    double ReadDouble()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::DOUBLE, "Not matched type corrected type is [DOUBLE]");

        return t.variant.doubleValue;
    }

    std::string ReadString()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        return *(t.variant.stringValue);
    }

    void ReadString(std::string& location)
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        location.clear();
        location.append(t.variant.stringValue->c_str());
    }

    void ReadRawValue(void* buffer)
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // RawValue때 kStringType을 넣었다.
        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        memcpy(buffer, t.variant.stringValue->c_str(), t.variant.stringValue->length());
    }

    void ReadKeyWithoutReturn()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");
    }

    std::string ReadKey()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");

        return *(t.variant.stringValue);
    }

    void ReadKeyWithCallback(void(callback(const char*, int*, int*, int*)), int* ke, int* s, int* e)
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");

        callback(t.variant.stringValue->c_str(), ke, s, e);
    }

    size_t ReadStartObject()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::START_OBJECT, "Not matched type corrected type is [START_OBJECT]");

        return t.count;
    }

    void ReadEndObject()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::END_OBJECT, "Not matched type corrected type is [END_OBJECT]");
    }

    size_t ReadStartArray()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::START_ARRAY, "Not matched type corrected type is [START_ARRAY]");

        return t.count;
    }

    void ReadEndArray()
    {
        const Token& t = _tokens.front();
        _tokens.pop_front();

        // LV_CHECK(t.type == TokenType::END_ARRAY, "Not matched type corrected type is [END_ARRAY]");
    }

    bool CheckEndArray()
    {
        const Token& t = _tokens.front();

        return (t.type == TokenType::END_ARRAY);
    }

    enum class TokenType
    {
        NONE,
        START_OBJECT,
        END_OBJECT,
        KEY,
        START_ARRAY,
        END_ARRAY,
        INT,
        UINT,
        DOUBLE,
        STRING,
        BOOL,
        NIL,
    };

    class Variant
    {
    public:

        enum class Type
        {
            PRIMITIVE,
            STRING
        };

        Type type;

        union
        {
            int64 intValue;
            uint64 uintValue;
            double doubleValue;
            bool boolValue;
            void* nullValue;

            // allocated variables
            std::string* stringValue;
        };

        Variant(int v) : type(Type::PRIMITIVE), intValue(v) { }
        Variant(uint32 v) : type(Type::PRIMITIVE), uintValue(v) { }
        Variant(int64 v) : type(Type::PRIMITIVE), intValue(v) { }
        Variant(uint64 v) : type(Type::PRIMITIVE), uintValue(v) { }
        Variant(double v) : type(Type::PRIMITIVE), doubleValue(v) { }
        Variant(bool v) : type(Type::PRIMITIVE), boolValue(v) { }
        Variant(void* v) : type(Type::PRIMITIVE), nullValue(v) { }
        Variant() : type(Type::PRIMITIVE), nullValue(nullptr) {};

        Variant(const std::string& v)
            : type(Type::STRING)
        {
            stringValue = new std::string(v);
        }
        Variant(std::string&& v)
            : type(Type::STRING)
        {
            stringValue = new  std::string(std::move(v));
        }

        ~Variant()
        {
            releaseAllocatedMemory();
        }

        void SetVariantInt(const Variant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input Variant Type");
            releaseAllocatedMemory();

            type = o.type;
            intValue = o.intValue;
        }
        void SetVariantUint(const Variant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input Variant Type");
            releaseAllocatedMemory();

            type = o.type;
            uintValue = o.uintValue;
        }
        void SetVariantDouble(const Variant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input Variant Type");
            releaseAllocatedMemory();

            type = o.type;
            doubleValue = o.doubleValue;
        }
        void SetVariantBool(const Variant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input Variant Type");
            releaseAllocatedMemory();

            type = o.type;
            boolValue = o.boolValue;
        }

        void SetVariantNil(const Variant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input Variant Type");
            releaseAllocatedMemory();

            type = o.type;
            nullValue = o.nullValue;
        }

        void SetVariantString(const Variant& o)
        {
            // LV_CHECK(o.type == Type::STRING, "Wrong Input Variant Type");

            if (this != &o)
            {
                releaseAllocatedMemory();

                type = o.type;
                stringValue = new std::string(*(o.stringValue));
            }
        }

        void SetVariantString(Variant&& o)
        {
            // LV_CHECK(o.type == Type::STRING, "Wrong Input Variant Type");

            if (this != &o)
            {
                releaseAllocatedMemory();
                type = o.type;
                stringValue = std::move(o.stringValue);

                o.type = Type::PRIMITIVE;
                o.stringValue = nullptr;
            }
        }


    private:
        void releaseAllocatedMemory()
        {
            switch (type)
            {
            case Type::STRING:
            {
                delete stringValue;
                stringValue = nullptr;
                break;
            }
            default: break;
            }
        }
    };

    struct Token
    {
        TokenType type;

        Variant variant;

        size_t count;

        template<typename T>
        Token(TokenType type, T&& t)
            : type(type)
            , variant(std::forward<T>(t))
        {}

        Token()
            : type(TokenType::NONE)
            , variant(nullptr)
        {}

        Token(const Token& o)
            : type(o.type)
            , count(o.count)
        {
            switch (type)
            {
            case TokenType::STRING:
            case TokenType::KEY:
                variant.SetVariantString(o.variant);
                break;
            case TokenType::INT:
                variant.SetVariantInt(o.variant);
                break;
            case TokenType::UINT:
                variant.SetVariantUint(o.variant);
                break;
            case TokenType::BOOL:
                variant.SetVariantBool(o.variant);
                break;
            case TokenType::DOUBLE:
                variant.SetVariantDouble(o.variant);
                break;
            case TokenType::NIL:
                variant.SetVariantNil(o.variant);
                break;
            default:
                variant.SetVariantBool(o.variant);
                break;
            }
        }

        Token(Token&& o) noexcept
            : type(o.type)
            , count(o.count)
        {
            switch (type)
            {
            case TokenType::STRING:
            case TokenType::KEY:
                variant.SetVariantString(std::move(o.variant));
                break;
            case TokenType::INT:
                variant.SetVariantInt(o.variant);
                break;
            case TokenType::UINT:
                variant.SetVariantUint(o.variant);
                break;
            case TokenType::BOOL:
                variant.SetVariantBool(o.variant);
                break;
            case TokenType::DOUBLE:
                variant.SetVariantDouble(o.variant);
                break;
            case TokenType::NIL:
                variant.SetVariantNil(o.variant);
                break;
            default:
                variant.SetVariantBool(o.variant);
                break;
            }
        }
    };

    std::deque<Token> _tokens;
};

struct ReadHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, ReadHandler>
{
    ReadHandler() {};

    virtual  ~ReadHandler()
    {

    }

    bool Null()
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::NIL, nullptr));
        return true;
    }

    bool Bool(bool b)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::BOOL, b));
        return true;
    }

    bool Int(int i)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::INT, i));
        return true;
    }

    bool Int64(int i)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::INT, i));
        return true;
    }

    bool Uint(uint32 u)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::UINT, u));
        return true;
    }

    bool Uint64(uint64 u)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::UINT, u));
        return true;
    }

    bool Double(double d)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::DOUBLE, d));
        return true;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy)
    {
        std::string string(str);
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::STRING, std::move(string)));
        return true;
    }

    bool StartObject()
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::START_OBJECT, true));
        return true;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy)
    {
        std::string string(str);
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::KEY, std::move(string)));
        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::END_OBJECT, true));
        return true;
    }

    bool StartArray()
    {
        context.push(reader._tokens.size());
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::START_ARRAY, true));
        return true;
    }

    bool EndArray(rapidjson::SizeType elementCount)
    {
        reader._tokens.push_back(JsonReader::Token(JsonReader::TokenType::END_ARRAY, true));
        size_t pos = context.top();
        context.pop();
        auto iter = reader._tokens.begin();
        for (size_t i = 0; i < pos; ++i)
        {
            iter++;
        }

        iter->count = elementCount;
        return true;
    }

    std::stack<size_t> context;
    JsonReader reader;
};

JsonSerializer::JsonSerializer() : Serializer(SerializeType::Write)
{
    _sbWriter = new rapidjson::StringBuffer();
    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)_sbWriter;
    _writer = new Writer(*sbWriter);
}


JsonSerializer::~JsonSerializer()
{
    Writer* writer = (Writer*)_writer;
    if (writer) delete writer;

    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)_sbWriter;
    if (sbWriter) delete sbWriter;

    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    if (readHandler) delete readHandler;
}

std::string JsonSerializer::GetResult()
{
    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)_sbWriter;
    return sbWriter->GetString();
}

void JsonSerializer::wStartObject()
{
    Writer* writer = (Writer*)_writer;
    _keyCounter++;
    writer->StartObject();
}

void JsonSerializer::wStartObject(TypeId type)
{
    wStartObject();
}

void JsonSerializer::wKey(const char* key)
{
    Writer* writer = (Writer*)_writer;
    if (0 == _keyCounter) return;

    _keyCounter++;
    writer->Key(key);
}

void JsonSerializer::write(const bool data)
{
    Writer* writer = (Writer*)_writer;
    writer->Bool(data);
}

void JsonSerializer::write(const int8 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Int(data);
}

void JsonSerializer::write(const uint8 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Uint(data);
}

void JsonSerializer::write(const int16 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Int(data);
}

void JsonSerializer::write(const uint16 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Uint(data);
}

void JsonSerializer::write(const int32 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Int(data);
}

void JsonSerializer::write(const uint32 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Uint(data);
}

void JsonSerializer::write(const int64 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Int64(data);
}

void JsonSerializer::write(const uint64 data)
{
    Writer* writer = (Writer*)_writer;
    writer->Uint64(data);
}

void JsonSerializer::write(const float data)
{
    Writer* writer = (Writer*)_writer;
    writer->Double(data);
}

void JsonSerializer::write(const double data)
{
    Writer* writer = (Writer*)_writer;
    writer->Double(data);
}

void JsonSerializer::write(const std::string& data)
{
    Writer* writer = (Writer*)_writer;
    writer->String(data.c_str());
}

constexpr char bufferMarkerZero = 'Z';
constexpr char bufferMarkerByte = 'B';
constexpr char bufferMarkerTransition = 'T';

void JsonSerializer::wBuffer(void* buffer, size_t size)
{
    char* src = (char*)buffer;
    int originSize = (int)size;
    int encodeSize = base64_encode_length(src, originSize) + 1;
    char* encodeResult = (char*)malloc(encodeSize);
    base64_encode(src, originSize, &encodeResult);

    Writer* writer = (Writer*)_writer;
    writer->String(encodeResult);

    free(encodeResult);
}

void JsonSerializer::wStartArray(uint64 arrayLength)
{
    Writer* writer = (Writer*)_writer;
    writer->StartArray();
}

void JsonSerializer::wEndArray()
{
    Writer* writer = (Writer*)_writer;
    writer->EndArray();
}

void JsonSerializer::wEndObject()
{
    Writer* writer = (Writer*)_writer;
    writer->EndObject();
}

JsonSerializer::JsonSerializer(const char* json) : Serializer(SerializeType::Read)
{
    ReadHandler* readHandler = new ReadHandler;
    _readHandler = readHandler;

    // rapidjson::Reader == json 을 실제 parse 하는 class
    rapidjson::Reader reader;

    // StringStream : provides a stream interface to read from the provided string.
    rapidjson::StringStream ss(json);

    // (*readHandler) dereferences the readHandler pointer, 
    // providing the actual ReadHandler object to be used during the parsing process.
    // The ReadHandler class likely contains custom callbacks or methods
    // that will be invoked by the rapidjson::Reader during the parsing process
    // to handle various events or actions.
    reader.Parse<rapidjson::kParseNanAndInfFlag>(ss, (*readHandler));
}

void JsonSerializer::rStartObject()
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    _keyCounter++;
    readHandler->reader.ReadStartObject();
}

void JsonSerializer::rStartObject(TypeId type)
{
    rStartObject();
}

void JsonSerializer::useKey(const char* key)
{
    if (0 == _keyCounter) return;

    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    _keyCounter++;
    readHandler->reader.ReadKeyWithoutReturn();
}

bool JsonSerializer::hasKey(const char* key)
{
    //json archive reture always true
    return true;
}

void JsonSerializer::rKey(char* key)
{
    if (0 == _keyCounter) return;

    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    _keyCounter++;

    std::string strKey = readHandler->reader.ReadKey();

    memcpy(key, (void*)(strKey.c_str()), strKey.length());
}

void JsonSerializer::read(bool& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadBool();
}

void JsonSerializer::read(int8& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadInt();
}

void JsonSerializer::read(uint8& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadUint();
}

void JsonSerializer::read(int16& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadInt();
}

void JsonSerializer::read(uint16& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadUint();
}

void JsonSerializer::read(int32& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadInt();
}

void JsonSerializer::read(uint32& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadUint();
}

void JsonSerializer::read(int64& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadInt64();
}

void JsonSerializer::read(uint64& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadUint64();
}

void JsonSerializer::read(float& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = (float)readHandler->reader.ReadDouble();
}

void JsonSerializer::read(double& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    data = readHandler->reader.ReadDouble();
}

void JsonSerializer::read(std::string& data)
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    readHandler->reader.ReadString(data);
}

void JsonSerializer::rBuffer(void* buffer, size_t size)
{
    char* dest = (char*)buffer;
    int originSize = (int)size;
    int encodeSize = base64_encode_length(dest, originSize);

    std::string encodeData;
    read(encodeData);

    base64_decode(const_cast<char*>(encodeData.c_str()), encodeSize, dest);

}

size_t JsonSerializer::rStartArray()
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    return readHandler->reader.ReadStartArray();
}

void JsonSerializer::rEndArray()
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    readHandler->reader.ReadEndArray();
}

void JsonSerializer::rEndObject()
{
    ReadHandler* readHandler = (ReadHandler*)_readHandler;
    readHandler->reader.ReadEndObject();
}
