#include "PrecompiledHeader.h"
#include "JsonSerializer.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/Reader.h>
#include <rapidjson/document.h>

#include "Encode.h"

// 구조
//         - JsonSerializer -> rapidJson 의 ReadHandler 객체 이용
//         - ReadHandler Custom 하게 사용하기
//         - ReadHandler Custom 내부에서는 JsonReader 라는 Class 사용

using JWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

// 각각의 {} 단위를 Token 으로 구분해서 데이터를 읽는다.
struct  JsonReader
{
    JsonReader() { }

    JsonReader(const JsonReader& o)
        : m_Tokens(o.m_Tokens)
    {
    }

    ~JsonReader()
    {
    }

    bool ReadBool()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // assert(t.type == TokenType::BOOL, "Not matched type corrected type is [BOOL]");
        return t.m_TokenVariant.m_BoolVal;
    }

    int ReadInt()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::INT || t.type == TokenType::UINT, "Not matched type corrected type is [INT]");
        return static_cast<int>(t.m_TokenVariant.m_IntVal);
    }

    int64 ReadInt64()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::INT || t.type == TokenType::UINT, "Not matched type corrected type is [INT]");

        return t.m_TokenVariant.m_IntVal;
    }

    uint32 ReadUint()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::UINT, "Not matched type corrected type is [UINT]");

        return static_cast<uint32>(t.m_TokenVariant.m_UintVal);
    }

    uint64 ReadUint64()
    {
        const Token& t = m_Tokens.front();

        // LV_CHECK(t.type == TokenType::UINT, "Not matched type corrected type is [UINT]");

        return t.m_TokenVariant.m_UintVal;
    }

    double ReadDouble()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::DOUBLE, "Not matched type corrected type is [DOUBLE]");

        return t.m_TokenVariant.m_DoubleVal;
    }

    std::string ReadString()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        return *(t.m_TokenVariant.m_StringVal);
    }

    void ReadString(std::string& location)
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        location.clear();
        location.append(t.m_TokenVariant.m_StringVal->c_str());
    }

    // string -> void* 형태로 read 한다.
    void ReadRawValue(void* buffer)
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::STRING, "Not matched type corrected type is [STRING]");

        memcpy(buffer, t.m_TokenVariant.m_StringVal->c_str(), t.m_TokenVariant.m_StringVal->length());
    }

    void ReadKeyWithoutReturn()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");
    }

    std::string ReadKey()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");

        return *(t.m_TokenVariant.m_StringVal);
    }

    void ReadKeyWithCallback(void(callback(const char*, int*, int*, int*)), int* ke, int* s, int* e)
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::KEY, "Not matched type corrected type is [KEY]");

        callback(t.m_TokenVariant.m_StringVal->c_str(), ke, s, e);
    }

    size_t ReadStartObject()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::START_OBJECT, "Not matched type corrected type is [START_OBJECT]");

        return t.m_Count;
    }

    void ReadEndObject()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::END_OBJECT, "Not matched type corrected type is [END_OBJECT]");
    }

    size_t ReadStartArray()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::START_ARRAY, "Not matched type corrected type is [START_ARRAY]");

        return t.m_Count;
    }

    void ReadEndArray()
    {
        const Token& t = m_Tokens.front();
        m_Tokens.pop_front();

        // LV_CHECK(t.type == TokenType::END_ARRAY, "Not matched type corrected type is [END_ARRAY]");
    }

    bool CheckEndArray()
    {
        const Token& t = m_Tokens.front();

        return (t.m_TokenType == TokenType::ENDARRAY);
    }

    enum class TokenType
    {
        NONE,
        INT,
        UINT,
        DOUBLE,
        STRING,
        BOOL,
        STARTOBJECT,
        ENDOBJECT,
        KEY,
        STARTARRAY,
        ENDARRAY,
        NIL,
    };

    class TokenVariant
    {
    public:

        enum class TokenVariantType
        {
            PRIMITIVE,
            STRING
        };

        TokenVariantType m_Type;

        union
        {
            int64 m_IntVal;
            uint64 m_UintVal;
            double m_DoubleVal;
            bool m_BoolVal;
            void* m_NullVal;

            // string 값은 char 포인터 형태로 들고있고자 하는 것 같다.
            std::string* m_StringVal;
        };

        TokenVariant(int v) : m_Type(TokenVariantType::PRIMITIVE), m_IntVal(v) { }
        TokenVariant(uint32 v) : m_Type(TokenVariantType::PRIMITIVE), m_UintVal(v) { }
        TokenVariant(int64 v) : m_Type(TokenVariantType::PRIMITIVE), m_IntVal(v) { }
        TokenVariant(uint64 v) : m_Type(TokenVariantType::PRIMITIVE), m_UintVal(v) { }
        TokenVariant(double v) : m_Type(TokenVariantType::PRIMITIVE), m_DoubleVal(v) { }
        TokenVariant(bool v) : m_Type(TokenVariantType::PRIMITIVE), m_BoolVal(v) { }
        TokenVariant(void* v) : m_Type(TokenVariantType::PRIMITIVE), m_NullVal(v) { }
        TokenVariant() : m_Type(TokenVariantType::PRIMITIVE), m_NullVal(nullptr) {};

        TokenVariant(const std::string& v)
            : m_Type(TokenVariantType::STRING)
        {
            m_StringVal = new std::string(v);
        }
        TokenVariant(std::string&& v)
            : m_Type(TokenVariantType::STRING)
        {
            m_StringVal = new  std::string(std::move(v));
        }

        ~TokenVariant()
        {
            releaseAllocatedMemory();
        }

        void SetTokenVariantInt(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input TokenVariant Type");
            releaseAllocatedMemory();

            m_Type = o.m_Type;
            m_IntVal = o.m_IntVal;
        }
        void SetTokenVariantUint(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input TokenVariant Type");
            releaseAllocatedMemory();

            m_Type = o.m_Type;
            m_UintVal = o.m_UintVal;
        }
        void SetTokenVariantDouble(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input TokenVariant Type");
            releaseAllocatedMemory();

            m_Type = o.m_Type;
            m_DoubleVal = o.m_DoubleVal;
        }
        void SetTokenVariantBool(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input TokenVariant Type");
            releaseAllocatedMemory();

            m_Type = o.m_Type;
            m_BoolVal = o.m_BoolVal;
        }

        void SetTokenVariantNil(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::PRIMITIVE, "Wrong Input TokenVariant Type");
            releaseAllocatedMemory();

            m_Type = o.m_Type;
            m_NullVal = o.m_NullVal;
        }

        void SetTokenVariantString(const TokenVariant& o)
        {
            // LV_CHECK(o.type == Type::STRING, "Wrong Input TokenVariant Type");

            if (this != &o)
            {
                releaseAllocatedMemory();

                m_Type = o.m_Type;
                m_StringVal = new std::string(*(o.m_StringVal));
            }
        }

        void SetTokenVariantString(TokenVariant&& o)
        {
            // LV_CHECK(o.type == Type::STRING, "Wrong Input TokenVariant Type");

            if (this != &o)
            {
                releaseAllocatedMemory();
                m_Type = o.m_Type;
                m_StringVal = std::move(o.m_StringVal);

                o.m_Type = TokenVariantType::PRIMITIVE;
                o.m_StringVal = nullptr;
            }
        }


    private:
        void releaseAllocatedMemory()
        {
            switch (m_Type)
            {
            case TokenVariantType::STRING:
            {
                delete m_StringVal;
                m_StringVal = nullptr;
                break;
            }
            default: break;
            }
        }
    };

    struct Token
    {
        TokenType m_TokenType;

        // 각 Token 이 가지고 있는 실제 Value
        TokenVariant m_TokenVariant;

        // Token Type 이 StartArray, EndArray 일 때
        // 해당 Array 에 몇개의 element 가 있는가에 대한 정보
        size_t m_Count;

        template<typename T>
        Token(TokenType type, T&& t)
            : m_TokenType(type)
            , m_TokenVariant(std::forward<T>(t))
        {}

        Token()
            : m_TokenType(TokenType::NONE)
            , m_TokenVariant(nullptr)
        {}

        Token(const Token& o)
            : m_TokenType(o.m_TokenType)
            , m_Count(o.m_Count)
        {
            switch (m_TokenType)
            {
            case TokenType::STRING:
            case TokenType::KEY:
                m_TokenVariant.SetTokenVariantString(o.m_TokenVariant);
                break;
            case TokenType::INT:
                m_TokenVariant.SetTokenVariantInt(o.m_TokenVariant);
                break;
            case TokenType::UINT:
                m_TokenVariant.SetTokenVariantUint(o.m_TokenVariant);
                break;
            case TokenType::BOOL:
                m_TokenVariant.SetTokenVariantBool(o.m_TokenVariant);
                break;
            case TokenType::DOUBLE:
                m_TokenVariant.SetTokenVariantDouble(o.m_TokenVariant);
                break;
            case TokenType::NIL:
                m_TokenVariant.SetTokenVariantNil(o.m_TokenVariant);
                break;
            default:
                m_TokenVariant.SetTokenVariantBool(o.m_TokenVariant);
                break;
            }
        }

        Token(Token&& o) noexcept
            : m_TokenType(o.m_TokenType)
            , m_Count(o.m_Count)
        {
            switch (m_TokenType)
            {
            case TokenType::STRING:
            case TokenType::KEY:
                m_TokenVariant.SetTokenVariantString(std::move(o.m_TokenVariant));
                break;
            case TokenType::INT:
                m_TokenVariant.SetTokenVariantInt(o.m_TokenVariant);
                break;
            case TokenType::UINT:
                m_TokenVariant.SetTokenVariantUint(o.m_TokenVariant);
                break;
            case TokenType::BOOL:
                m_TokenVariant.SetTokenVariantBool(o.m_TokenVariant);
                break;
            case TokenType::DOUBLE:
                m_TokenVariant.SetTokenVariantDouble(o.m_TokenVariant);
                break;
            case TokenType::NIL:
                m_TokenVariant.SetTokenVariantNil(o.m_TokenVariant);
                break;
            default:
                m_TokenVariant.SetTokenVariantBool(o.m_TokenVariant);
                break;
            }
        }
    };

    std::deque<Token> m_Tokens;
};

struct ReadHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, ReadHandler>
{
    ReadHandler() {};

    virtual  ~ReadHandler()
    {

    }

    bool Null()
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::NIL, nullptr));
        return true;
    }

    bool Bool(bool b)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::BOOL, b));
        return true;
    }

    bool Int(int i)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::INT, i));
        return true;
    }

    bool Int64(int i)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::INT, i));
        return true;
    }

    bool Uint(uint32 u)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::UINT, u));
        return true;
    }

    bool Uint64(uint64 u)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::UINT, u));
        return true;
    }

    bool Double(double d)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::DOUBLE, d));
        return true;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy)
    {
        std::string string(str);
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::STRING, std::move(string)));
        return true;
    }

    bool StartObject()
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::STARTOBJECT, true));
        return true;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy)
    {
        std::string string(str);
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::KEY, std::move(string)));
        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::ENDOBJECT, true));
        return true;
    }

    bool StartArray()
    {
        m_History.push(m_Reader.m_Tokens.size());
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::STARTARRAY, true));
        return true;
    }

    bool EndArray(rapidjson::SizeType elementCount)
    {
        m_Reader.m_Tokens.push_back(JsonReader::Token(JsonReader::TokenType::ENDARRAY, true));
        
        // StartArray() 함수에서 넣어준 element 개수 정보
        size_t pos = m_History.top();
        m_History.pop();
        auto iter = m_Reader.m_Tokens.begin();
        for (size_t i = 0; i < pos; ++i)
        {
            iter++;
        }

        iter->m_Count = elementCount;
        return true;
    }

    std::stack<size_t> m_History;
    JsonReader m_Reader;
};

JsonSerializer::JsonSerializer() : Serializer(SerializeType::Write)
{
    m_StringBuffer = new rapidjson::StringBuffer();
    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)m_StringBuffer;
    m_JWriter = new JWriter(*sbWriter);
}


JsonSerializer::~JsonSerializer()
{
    JWriter* writer = (JWriter*)m_JWriter;
    if (writer) delete writer;

    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)m_StringBuffer;
    if (sbWriter) delete sbWriter;

    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    if (readHandler) delete readHandler;
}

std::string JsonSerializer::GetFinalResult()
{
    rapidjson::StringBuffer* sbWriter = (rapidjson::StringBuffer*)m_StringBuffer;
    return sbWriter->GetString();
}

void JsonSerializer::wStartObject()
{
    JWriter* writer = (JWriter*)m_JWriter;
    m_KeyCounter++;
    writer->StartObject();
}

void JsonSerializer::wStartObject(TypeId type)
{
    wStartObject();
}

void JsonSerializer::wKey(const char* key)
{
    JWriter* writer = (JWriter*)m_JWriter;
    if (0 == m_KeyCounter) return;

    m_KeyCounter++;
    writer->Key(key);
}

void JsonSerializer::write(const bool data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Bool(data);
}

void JsonSerializer::write(const int8 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Int(data);
}

void JsonSerializer::write(const uint8 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Uint(data);
}

void JsonSerializer::write(const int16 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Int(data);
}

void JsonSerializer::write(const uint16 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Uint(data);
}

void JsonSerializer::write(const int32 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Int(data);
}

void JsonSerializer::write(const uint32 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Uint(data);
}

void JsonSerializer::write(const int64 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Int64(data);
}

void JsonSerializer::write(const uint64 data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Uint64(data);
}

void JsonSerializer::write(const float data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Double(data);
}

void JsonSerializer::write(const double data)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->Double(data);
}

void JsonSerializer::write(const std::string& data)
{
    JWriter* writer = (JWriter*)m_JWriter;
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

    // encodeResult : byte data 를 char 형태로 변환
    JWriter* writer = (JWriter*)m_JWriter;
    writer->String(encodeResult);

    free(encodeResult);
}

void JsonSerializer::wStartArray(uint64 arrayLength)
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->StartArray();
}

void JsonSerializer::wEndArray()
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->EndArray();
}

void JsonSerializer::wEndObject()
{
    JWriter* writer = (JWriter*)m_JWriter;
    writer->EndObject();
}

JsonSerializer::JsonSerializer(const char* json) : Serializer(SerializeType::Read)
{
    ReadHandler* readHandler = new ReadHandler;
    m_JReader = readHandler;

    // rapidjson::m_Reader == json 을 실제 parse 하는 class
    rapidjson::Reader reader;

    // StringStream : provides a stream interface to read from the provided string.
    rapidjson::StringStream ss(json);

    // (*readHandler) dereferences the readHandler pointer, 
    // providing the actual ReadHandler object to be used during the parsing process.
    // The ReadHandler class likely contains custom callbacks or methods
    // that will be invoked by the rapidjson::m_Reader during the parsing process
    // to handle various events or actions.
    reader.Parse<rapidjson::kParseNanAndInfFlag>(ss, (*readHandler));
}

void JsonSerializer::rStartObject()
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    m_KeyCounter++;
    readHandler->m_Reader.ReadStartObject();
}

void JsonSerializer::rStartObject(TypeId type)
{
    rStartObject();
}

void JsonSerializer::useKey(const char* key)
{
    if (0 == m_KeyCounter) return;

    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    m_KeyCounter++;
    readHandler->m_Reader.ReadKeyWithoutReturn();
}

bool JsonSerializer::hasKey(const char* key)
{
    //json archive reture always true
    return true;
}

void JsonSerializer::rKey(char* key)
{
    if (0 == m_KeyCounter) return;

    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    m_KeyCounter++;

    std::string strKey = readHandler->m_Reader.ReadKey();

    memcpy(key, (void*)(strKey.c_str()), strKey.length());
}

void JsonSerializer::read(bool& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadBool();
}

void JsonSerializer::read(int8& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadInt();
}

void JsonSerializer::read(uint8& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadUint();
}

void JsonSerializer::read(int16& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadInt();
}

void JsonSerializer::read(uint16& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadUint();
}

void JsonSerializer::read(int32& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadInt();
}

void JsonSerializer::read(uint32& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadUint();
}

void JsonSerializer::read(int64& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadInt64();
}

void JsonSerializer::read(uint64& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadUint64();
}

void JsonSerializer::read(float& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = (float)readHandler->m_Reader.ReadDouble();
}

void JsonSerializer::read(double& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    data = readHandler->m_Reader.ReadDouble();
}

void JsonSerializer::read(std::string& data)
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    readHandler->m_Reader.ReadString(data);
}

void JsonSerializer::rBuffer(void* buffer, size_t size)
{
    char* dest = (char*)buffer;
    int originSize = (int)size;
    int encodeSize = base64_encode_length(dest, originSize);

    std::string encodeData;
    read(encodeData);

    //buffer 
    base64_decode(const_cast<char*>(encodeData.c_str()), encodeSize, dest);

}

size_t JsonSerializer::rStartArray()
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    return readHandler->m_Reader.ReadStartArray();
}

void JsonSerializer::rEndArray()
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    readHandler->m_Reader.ReadEndArray();
}

void JsonSerializer::rEndObject()
{
    ReadHandler* readHandler = (ReadHandler*)m_JReader;
    readHandler->m_Reader.ReadEndObject();
}
