#pragma once

#include <filesystem>

namespace Eclipse
{
	enum class CompileResult
	{
		Success,
		CompileError,
		FileNotFound,
		LinkageFailed
	};

	struct ShaderCompileInfo
	{
		CompileResult status;
		unsigned int shaderID;
	};

	class ShaderCompiler
	{
	public:
		static ShaderCompileInfo CompileShaderFromPath(unsigned int shaderType, const std::filesystem::path& aPath);
		static ShaderCompileInfo CompileShaderFromMemory(unsigned int shaderType, const char* src);

		static ShaderCompileInfo CreateShaderProgram(unsigned int fragmentShader, unsigned int vertexShader);
	};
}