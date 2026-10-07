#pragma once

#include "Core.Core.hpp"
#include "Types.hpp"

namespace Eclipse
{
	CORE_API void* MemSet(void* destination, U8 value, Size size);

	CORE_API bool IsPowerOf2(Size size);
}

