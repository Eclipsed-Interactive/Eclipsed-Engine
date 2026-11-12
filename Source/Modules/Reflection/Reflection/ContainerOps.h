#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Core/Standard/TypeIDGenerator.hpp"

#include "Runtime/ReflectionFlags.h"

#include "FieldEntry.h"
#include "Reflection.Core.hpp"

namespace Eclipse::Reflection
{
	struct REFLECTION_API ContainerOps
	{
		TypeID elementType;

		size_t(*size)(const void* container) = nullptr;
		void* (*at)(void* container, size_t index) = nullptr;
		const void* (*atConst)(const void* container, size_t index) = nullptr;
		void (*resize)(void* container, size_t newSize) = nullptr;
		void (*erase)(void* container, size_t index) = nullptr;
	};
}