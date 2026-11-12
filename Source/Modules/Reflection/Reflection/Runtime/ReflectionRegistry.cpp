#include "ReflectionRegistry.h"

#include <stdexcept>

namespace Eclipse::Reflection
{
	void ReflectionRegistry::RegisterType(TypeEntry type)
	{
		typeNameToID.insert_or_assign(type.name, type.id);
		IdToType.insert_or_assign(type.id, std::move(type));
	}

	void ReflectionRegistry::RegisterField(const std::string& typeName, FieldEntry field)
	{
		if (typeNameToID.contains(typeName))
		{
			TypeEntry& entry = IdToType[typeNameToID[typeName]];
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

	const FieldEntry& ReflectionRegistry::GetField(TypeID type, const std::string& fieldName)
	{
		return IdToType[type].fields[fieldName];
	}

	const TypeEntry& ReflectionRegistry::GetType(TypeID aID)
	{
		auto it = IdToType.find(aID);
		return it != IdToType.end() ? it->second : throw;
	}

	const TypeEntry& ReflectionRegistry::GetType(const std::string& typeName)
	{
		auto it = typeNameToID.find(typeName);
		return it != typeNameToID.end() ? GetType(it->second) : throw;
	}

	const std::unordered_map<TypeID, TypeEntry>& ReflectionRegistry::GetTypes()
	{
		return IdToType;
	}




	const TypeEntry* ReflectionRegistry::TryGetType(TypeID aID)
	{
		auto it = IdToType.find(aID);
		return it != IdToType.end() ? &it->second : nullptr;
	}
	
	const TypeEntry* ReflectionRegistry::TryGetType(const std::string& typeName)
	{
		auto it = typeNameToID.find(typeName);
		return it != typeNameToID.end() ? TryGetType(it->second) : nullptr;
	}

	const ContainerOps* ReflectionRegistry::TryGetContainer(TypeID id)
	{
		auto it = containers.find(id);
		return it != containers.end() ? &it->second : nullptr;
	}

	void ReflectionRegistry::RegisterContainer(TypeID aTypeId, ContainerOps aContainerOps)
	{
		containers.insert_or_assign(aTypeId, std::move(aContainerOps));
	}
}