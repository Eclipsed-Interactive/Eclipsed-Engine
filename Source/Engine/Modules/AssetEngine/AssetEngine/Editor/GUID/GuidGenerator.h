#pragma once

#include <unordered_set>
#include <string>
#include <filesystem>

namespace Eclipse
{
	class GuidGenerator
	{
	public:
		static size_t Generate(const std::string& someData);
	};
}