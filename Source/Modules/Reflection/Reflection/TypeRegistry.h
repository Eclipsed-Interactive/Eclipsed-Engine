#pragma once

#include "TypeDescriptor.h"

#include <unordered_map>
#include <string>

namespace Eclipse::Reflection
{
	class TypeRegistry
	{
	public:
		static void Register(const TypeDescriptor& type);
		static const TypeDescriptor& Get(const std::string& name);

		static const std::unordered_map<std::string, TypeDescriptor>& GetAll();

		static inline std::unordered_map<std::string, TypeDescriptor> reg;
	};
}