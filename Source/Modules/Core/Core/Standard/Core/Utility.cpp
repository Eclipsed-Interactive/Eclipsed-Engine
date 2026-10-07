#include "Asserts.hpp"
#include "Utility.hpp"

namespace Eclipse
{
	CORE_API void* MemSet(void* destination, U8 value, Size size)
	{
		U8* bytes = static_cast<U8*>(destination);

		for (Size i = 0; i < size; i++)
		{
			bytes[i] = value;
		}

		return destination;
	}

	CORE_API bool IsPowerOf2(Size size)
	{
		return (size & (size - 1)) == 0;
	}
}