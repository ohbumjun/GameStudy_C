// rapidjson/example/simpledom/simpledom.cpp`
#include "PrecompiledHeader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "JsonDomSerializer.h"

using namespace rapidjson;

int main() {

    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";

    JsonDomSerializer jds(json);

    return 0;
}