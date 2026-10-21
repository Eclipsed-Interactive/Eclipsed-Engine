#pragma once

#include <string>
#include <unordered_map>

#include "Reflection/Attributes/AttributeContainer.h"
#include "Core/Standard/TypeIDGenerator.hpp"

#include "ReflectionFlags.h"

#include "Reflection.Core.hpp"

namespace Eclipse::Reflection
{
	struct TypeEntry;
	struct REFLECTION_API FieldEntry
	{
		std::string name;
		size_t offset;
		size_t size;

		AttributeContainer attributeContainer;
		TypeID type;
	};

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
	};

	class REFLECTION_API ReflectionRegistry
	{
	public:
		static void RegisterType(TypeEntry type);
		static void RegisterField(const std::string& typeName, FieldEntry field);

		static size_t GetOffset(const std::string& typeName, const std::string& fieldName);

	public:
		static const FieldEntry& GetField(const std::string& typeName, const std::string& fieldName);
		static const TypeEntry& GetType(const std::string& typeName);

		static const std::unordered_map<std::string, TypeEntry>& GetTypes();

		static const TypeEntry* TryGetType(const std::string& typeName);

	public:
		template<typename T>
		static TypeEntry* GetType();

	private:
		static inline std::unordered_map<std::string, TypeEntry> registererd;
	};
}

#include "TypeEntries.h"