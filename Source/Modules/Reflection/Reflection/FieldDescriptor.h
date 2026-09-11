#pragma once

#include <string>
#include <vector>
#include <functional>


namespace Eclipse::Reflection
{
	struct AttributeDescriptor {
		std::string Name;
		std::string Arguments;
	};

	struct TypeDescriptor;
	struct FieldDescriptor
	{
		std::string Name;
		std::string Type;

		std::vector<AttributeDescriptor> attributes;
	};
}