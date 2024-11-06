
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

int main()
{
    std::vector<uint8_t> myVector = { 1, 2, 3, 4 };

    const char* charArray = reinterpret_cast<const char*>(myVector.data());
    uint8_t* _sendBuffer = new uint8_t[10];
    memset(_sendBuffer, 0, 10);
    memcpy(_sendBuffer, charArray, myVector.size());
    std::cout << "original : " << myVector[0] << std::endl;
    std::cout << "copy : " << _sendBuffer[0] << std::endl;
    return 0;
}