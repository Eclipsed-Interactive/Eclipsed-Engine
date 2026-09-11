#include "ReflectionRegistry.h"

namespace Eclipse::Reflection
{
	void ReflectionRegistry::RegisterField(const std::string& typeName, const std::string& fieldName, size_t offset)
	{
		registererd[typeName][fieldName] = offset;
	}

	size_t ReflectionRegistry::GetOffset(const std::string& typeName, const std::string& fieldName)
	{
		auto fieldsIt = registererd.find(typeName);
		if (fieldsIt == registererd.end()) return 0;

		auto& fields = fieldsIt->second;

		auto field = fields.find(fieldName);
		if (field == fields.end()) return 0;

		return field->second;
	}
}