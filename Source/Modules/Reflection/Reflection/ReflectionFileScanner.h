#pragma once

#include <filesystem>
#include <string>
#include "Reflection.Core.hpp"

namespace Eclipse::Reflection
{
	class REFLECTION_API ReflectionFileScanner final 
	{
	public:
		static void ScanAndReflectFiles(const std::filesystem::path& root);
		static void ReflectFile(const std::filesystem::path& root);

	private:
		static std::string ReadFile(const std::filesystem::path& root);
	};
}