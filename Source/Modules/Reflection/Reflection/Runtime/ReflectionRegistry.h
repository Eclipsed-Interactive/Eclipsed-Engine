#pragma once

#include <string>
#include <unordered_map>

#include "Reflection.Core.hpp"

namespace Eclipse::Reflection
{
	struct REFLECTION_API FieldEntry
	{
		std::string name;
		size_t offset;
		size_t size;
	};

	struct REFLECTION_API TypeEntry
	{
		std::string name;
		size_t size;
		unsigned int id;

		void* (*create)() = nullptr;
		void (*destroy)(void*) = nullptr;

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

	private:
		static inline std::unordered_map<std::string, TypeEntry> registererd;
	};
}