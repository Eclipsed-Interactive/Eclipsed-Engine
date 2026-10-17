#include "Reflection.hpp"

#include "Core/PathManager.h"
#include "ReflectionFileScanner.h"

#include "TypeRegistry.h"
#include "ReflectionGenerator.h"

#include "Runtime/ReflectionRegistry.h"

namespace Eclipse::Reflection
{
	void Reflect(const std::filesystem::path& content, const std::filesystem::path& destination)
	{
		ReflectionFileScanner::ScanAndReflectFiles(content);

		ReflectionGenerator::Clear(destination);
		ReflectionGenerator::Generate(destination);
	}

	const TypeEntry& Find(const std::string& typeName)
	{
		try
		{
			const TypeEntry& type = ReflectionRegistry::GetType(typeName);
			return type;
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}

	const FieldEntry& Find(const std::string& typeName, const std::string& fieldName)
	{
		try
		{
			const FieldEntry& type = ReflectionRegistry::GetField(typeName, fieldName);
			return type;
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}

	void Set(const std::string& typeName, const std::string& fieldName, void* obj, const void* data)
	{
		try
		{
			const FieldEntry& field = ReflectionRegistry::GetField(typeName, fieldName);
			::memcpy(reinterpret_cast<char*>(obj) + field.offset, data, field.size);
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}

	void* Get(const std::string& typeName, const std::string& fieldName, void* obj)
	{
		try
		{
			const FieldEntry& field = ReflectionRegistry::GetField(typeName, fieldName);
			return reinterpret_cast<char*>(obj) + field.offset;
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}
}