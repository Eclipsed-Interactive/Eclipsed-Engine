#pragma once

#include <filesystem>

namespace Eclipse::Reflection
{
	class TypeDescriptor;
	class ReflectionGenerator final
	{
	public:
		static void Clear(const std::filesystem::path& path);
		static void Generate(const std::filesystem::path& path);

	private:
		static void GenerateFile(const std::filesystem::path& generationPath, const TypeDescriptor& descriptor);
		static void GenerateRegistratorFile(const std::filesystem::path& generationPath);
	};
}