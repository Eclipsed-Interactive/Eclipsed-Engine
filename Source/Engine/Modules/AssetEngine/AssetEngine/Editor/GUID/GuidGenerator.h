#pragma once

#include <unordered_set>
#include <string>
#include <filesystem>

namespace Eclipse
{
	class GuidGenerator
	{
	public:
		static std::string Generate();
		static size_t GetGuidFromPath(const std::string path);
	};
}