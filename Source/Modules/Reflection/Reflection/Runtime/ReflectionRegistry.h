#pragma once

#include <string>
#include <unordered_map>

namespace Eclipse::Reflection
{
	class ReflectionRegistry
	{
	public:
		static void RegisterField(const std::string& typeName, const std::string& fieldName, size_t offset);

		static size_t GetOffset(const std::string& typeName, const std::string& fieldName);

	private:
		static inline std::unordered_map<std::string, std::unordered_map<std::string, size_t>> registererd;
	};
}