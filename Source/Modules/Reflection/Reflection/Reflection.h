#pragma once

#include "Reflection.Core.hpp"
#include <filesystem>

#include "Runtime/ReflectionRegistry.h" // Remove when TypeEntry is in its own file.

namespace Eclipse::Reflection
{
	REFLECTION_API void Reflect(const std::filesystem::path& content, const std::filesystem::path& destination);

	REFLECTION_API const TypeEntry& Find(const std::string& typeName);
	REFLECTION_API const FieldEntry& Find(const std::string& typeName, const std::string& fieldName);

	REFLECTION_API void  Set(const std::string& typeName, const std::string& fieldName, void* obj, const void* data);
	REFLECTION_API void* Get(const std::string& typeName, const std::string& fieldName, void* obj);

	template<typename T>
	inline void Set(const std::string& typeName, const std::string& fieldName, void* obj, T data)
	{
		try
		{
			const FieldEntry& field = ReflectionRegistry::GetField(typeName, fieldName);
			::memcpy(reinterpret_cast<char*>(obj) + field.offset, &data, field.size);
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}

	template<typename T>
	inline T& Get(const std::string& typeName, const std::string& fieldName, void* obj)
	{
		try
		{
			const FieldEntry& field = ReflectionRegistry::GetField(typeName, fieldName);
			T* dataPtr = reinterpret_cast<T*>(reinterpret_cast<char*>(obj) + field.offset);
			return *dataPtr;
		}
		catch (const std::exception&)
		{
			// Silently fails.
		}
	}
}