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
	struct REFLECTION_API TypeEntry
	{
		std::string name;
		size_t size;
		TypeID id;

		void* (*create)() = nullptr;
		void* (*createAtAddress)(unsigned char*) = nullptr;
		void (*destroy)(void*) = nullptr;

		ReflectionFlags flag;
		std::unordered_map<std::string, FieldEntry> fields;
		std::vector<std::string> fieldOrder; 
	};
}