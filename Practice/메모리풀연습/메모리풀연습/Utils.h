#pragma once

#include <cstddef>

using namespace std;

class Utils {
public :
	static const size_t CalculatePadding(const size_t baseAddress,
		const size_t alignment)
{
		const size_t multiplier = (baseAddress / alignment) + 1;
		const size_t padding = (multiplier * alignment) - baseAddress;
		return padding;
}
	static const size_t CalculatePaddingWithHeader(const size_t baseAddress,
	const size_t alignment, const size_t HeaderSize)
{
	size_t padding = CalculatePadding(baseAddress, alignment);

	if (HeaderSize > padding)
	{
		size_t neededSpace = HeaderSize - padding;

		if (neededSpace % alignment > 0)
			padding += (alignment * (neededSpace / alignment + 1));
		else
			padding += (alignment * (neededSpace / alignment));
	}
	return padding;
}
};