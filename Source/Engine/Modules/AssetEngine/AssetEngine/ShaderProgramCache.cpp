#include "ShaderProgramCache.h"

#include "AssetEngine/Helper/ShaderCompiler.h"

namespace Eclipse
{
	unsigned int ShaderProgramCache::LoadOrGet(unsigned int shaderType, const std::filesystem::path& aPath)
	{
		if (Exists(aPath))
		{
			return Get(aPath);
		}
		else
		{
			return Load(shaderType, aPath);
		}
	}

	unsigned int ShaderProgramCache::Get(const std::filesystem::path& aPath)
	{
		return shaderCache[aPath.generic_string()];
	}
	
	unsigned int ShaderProgramCache::Load(unsigned int shaderType, const std::filesystem::path& aPath)
	{
		ShaderCompileInfo info = ShaderCompiler::CompileShaderFromPath(shaderType, aPath);
		if (info.status != CompileResult::Success) return 0;

		shaderCache[aPath.generic_string()] = info.shaderID;

		return info.shaderID;
	}

	bool ShaderProgramCache::Exists(const std::filesystem::path& aPath)
	{
		return shaderCache.find(aPath.generic_string()) != shaderCache.end();
	}
}