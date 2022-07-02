
#include <cstddef>	

using namespace std;

class Utils
{
public:
    static const size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment)
    {
        const size_t multiplier = (baseAddress / alignment) + 1;
        const size_t alignedAddress = multiplier * alignment;
        const size_t padding = alignedAddress - baseAddress;

        return padding;
    }

    static const size_t CalculatePaddingWithHeader(const size_t baseAddress,
        const size_t alignment, const size_t headerSize)
    {
        size_t padding = CalculatePadding(baseAddress, alignment);

        size_t neededSpace = headerSize;
        if (headerSize > padding)
        {
            neededSpace -= padding;

            if (neededSpace % alignment > 0)
                padding += (alignment * (1 + (neededSpace / alignment)));
            else
                padding += (alignment * (neededSpace / alignment));
        }

        return padding;
    }
};