#pragma once

#include <filesystem>
#include <unordered_map>

namespace Eclipse
{
	class ShaderProgramCache
	{
	public:
		unsigned int LoadOrGet(unsigned int shaderType, const std::filesystem::path& aPath);

	private:
		unsigned int Get(const std::filesystem::path& aPath);
		unsigned int Load(unsigned int shaderType, const std::filesystem::path& aPath);
		bool Exists(const std::filesystem::path& aPath);

	private:
		// Shader path to shader program id.
		std::unordered_map<std::string, unsigned int> shaderCache;
	};
}