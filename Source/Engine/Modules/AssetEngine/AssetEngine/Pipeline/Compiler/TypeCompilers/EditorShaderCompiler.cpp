#include "EditorShaderCompiler.h"
#include <fstream>
#include <cereal/archives/json.hpp>

#include "AssetEngine/Helper/ShaderCompiler.h"
#include "EditorShaderCompiler.h"

namespace Eclipse::Assets
{
	void ShaderCompiler::Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData)
	{
		std::ifstream in(meta.fullPath);



		std::string vertexShaderSource;
		std::string pixelShaderSource;

		enum class ActiveShader
		{
			Vertex, Pixel, None
		};

		std::string line;
		ActiveShader activeShader = ActiveShader::None;
		while (std::getline(in, line))
		{
			if (line == "#shader vertex")
			{
				activeShader = ActiveShader::Vertex;
			}
			else if (line == "#shader vertex" || line == "#shader fragment")
			{
				activeShader = ActiveShader::Pixel;
			}


			switch (activeShader)
			{
			case ActiveShader::Vertex: vertexShaderSource += line + "\n";
				break;
			case ActiveShader::Pixel: pixelShaderSource += line + "\n";
				break;
			}
		}


		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSource = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);

		GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* pixelSource = pixelShaderSource.c_str();
		glShaderSource(pixelShader, 1, &pixelSource, nullptr);
		glCompileShader(pixelShader);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, pixelShader);
		glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
		glLinkProgram(program);

		GLint binaryLength = 0;
		glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

		if (binaryLength == 0)
		{
			std::vector<GLubyte> binary(binaryLength);
			GLenum binaryFormat = 0;

			glGetProgramBinary(program, binaryLength, nullptr, &binaryFormat, binary.data());

			AddToBinaryData(&binaryFormat, sizeof(binaryFormat), binaryData);
			AddToBinaryData(&binaryLength, sizeof(binaryLength), binaryData);
			AddToBinaryData(binary.data(), binaryLength, binaryData);
		}
	}
}