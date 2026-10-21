#pragma once

#include <string>
#include <vector>

#include "FieldDescriptor.h"
#include "Runtime/ReflectionFlags.h"

namespace Eclipse::Reflection
{
	struct TypeDescriptor
	{
		std::string Name;
		std::string FilePath;
		ReflectionFlags flag;

		std::vector<FieldDescriptor> Fields;
	};
}