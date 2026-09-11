#include "TypeRegistry.h"

namespace Eclipse::Reflection
{
	void TypeRegistry::Register(const TypeDescriptor& type)
	{
		reg[type.Name] = type;
	}

	const TypeDescriptor& Eclipse::Reflection::TypeRegistry::Get(const std::string& name)
	{
		return reg[name];
	}
	const std::unordered_map<std::string, TypeDescriptor>& TypeRegistry::GetAll()
	{
		return reg;
	}
}