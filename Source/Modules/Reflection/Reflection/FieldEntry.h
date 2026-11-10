#pragma once

#include <string>

#include "Reflection/Attributes/AttributeContainer.h"
#include "Core/Standard/TypeIDGenerator.hpp"
#include "Reflection.Core.hpp"
#include "FieldEntry.h"

#include "Runtime/ReflectionFlags.h"

namespace Eclipse::Reflection
{
	struct REFLECTION_API FieldEntry
	{
		std::string name;
		std::string owner;
		size_t offset;
		size_t size;

		AttributeContainer attributeContainer;
		TypeID type;
	};
}