#pragma once

#include <string>
#include <vector>

#include "FieldDescriptor.h"

namespace Eclipse::Reflection
{
	struct TypeDescriptor
	{
		std::string Name;
		std::string FilePath;
		std::vector<FieldDescriptor> Fields;
	};
}