#pragma once

#include <filesystem>

namespace Eclipse::Reflection
{
	class TypeDescriptor;
	class ReflectionGenerator final
	{
	public:
		static void Clear(const std::filesystem::path& path);
		static void Generate(const std::filesystem::path& path, bool useStatic);

	private:
		static void GenerateFile(const std::filesystem::path& generationPath, const TypeDescriptor& descriptor);
		static void GenerateRegistratorFileDynamicInitiator(const std::filesystem::path& generationPath);
		static void GenerateRegistratorFileStaticInitiator(const std::filesystem::path& generationPath);
	};
}