#include "ReflectionRegistry.h"

#include <stdexcept>

namespace Eclipse::Reflection
{
	void ReflectionRegistry::RegisterType(TypeEntry type)
	{
		registererd.insert_or_assign(type.name, std::move(type));
	}

	void ReflectionRegistry::RegisterField(const std::string& typeName, FieldEntry field)
	{
		if (registererd.contains(typeName))
		{
			TypeEntry& entry = registererd[typeName];
			entry.fields.insert_or_assign(field.name, std::move(field));
		}
	}

	size_t ReflectionRegistry::GetOffset(const std::string& typeName, const std::string& fieldName)
	{
		const FieldEntry& field = GetField(typeName, fieldName);
		return field.offset;
	}

	const FieldEntry& ReflectionRegistry::GetField(const std::string& typeName, const std::string& fieldName)
	{
		const TypeEntry& type = GetType(typeName);

		if (!type.fields.contains(fieldName))
			throw std::runtime_error("The field dont exist.");

		return type.fields.at(fieldName);
	}

	const TypeEntry& ReflectionRegistry::GetType(const std::string& typeName)
	{
		if (!registererd.contains(typeName))
			throw std::runtime_error("The type dont exist.");

		return registererd[typeName];
	}
}