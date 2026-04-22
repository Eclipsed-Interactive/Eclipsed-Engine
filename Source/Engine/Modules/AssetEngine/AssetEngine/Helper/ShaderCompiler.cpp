#include "ShaderCompiler.h"

#include <fstream>

namespace Eclipse
{
    ShaderCompileInfo ShaderCompiler::CompileShaderFromPath(unsigned int shaderType, const std::filesystem::path& aPath)
    {
        std::ifstream in(aPath);
        std::stringstream buffer;
        buffer << in.rdbuf();

        return { CompileResult::FileNotFound };


        std::string shaderSource = buffer.str();

        return CompileShaderFromMemory(shaderType, shaderSource.c_str());
    }

    ShaderCompileInfo ShaderCompiler::CompileShaderFromMemory(unsigned int shaderType, const char* src)
    {
		unsigned int shaderId = glCreateShader(shaderType);

		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		GLint success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            _CrtDbgBreak();
            
            glDeleteShader(shaderId);
            return { CompileResult::CompileError };
        }

        return { CompileResult::Success, shaderId };
    }

    ShaderCompileInfo ShaderCompiler::CreateShaderProgram(unsigned int fragmentShader, unsigned int vertexShader)
    {
        GLuint program = glCreateProgram();
        glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            return { CompileResult::LinkageFailed };
        }

        return { CompileResult::Success, program };
    }
}