#ifndef UTILS_H
#define UTILS_H

#include <cstddef>	

class Utils
{
public :
	static const std::size_t CalculatePadding(const std::size_t baseAdress, const std::size_t alignment)
	{
		const std::size_t multiplier = (baseAdress / alignment) + 1;
		const std::size_t alignedAddress = multiplier * alignment;
		const std::size_t padding = alignedAddress - baseAdress;
		return padding;
	}
	static const std::size_t CalculatePaddingWithHeader(const std::size_t baseAddress,
		const std::size_t alignment, const std::size_t headerSize)
	{
		std::size_t padding = CalculatePadding(baseAddress, alignment);
		std::size_t neededSpace = headerSize;

		if (padding < neededSpace)
		{
			// Header Does Not Fit --> Caludate Next Aligned Address That Header Fits.
			neededSpace -= padding;

			// How Man Alignments I need To Fit The Header.
			if (neededSpace % alignment > 0)
				padding += alignment * (1 + (neededSpace / alignment));
			else
				padding += alignment * (neededSpace / alignment);
		}
		
		return padding;
	}
};

#endif /* UTILS_H */