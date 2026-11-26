#include "ShaderImporter.h"

#include "AssetSystem/Import/ImportContext.h"
#include "Core/Helpers/STB_Helper.h"

#include "AssetSystem/MetaData/TextureMeta.h"

#include "AssetSystem/ShaderType.h"
#include "Core/Logger/LoggerMacros.h"

#include <OpenGL/glslang/Public/ShaderLang.h>
#include <OpenGL/glslang/Public/ResourceLimits.h>
#include <OpenGL/glslang/SPIRV/GlslangToSpv.h>

namespace Eclipse::AssetSystem
{
	namespace
	{
		ShaderType StripShaderType(std::string& aShaderContent)
		{
			constexpr std::string_view tag = "#type ";
			if (aShaderContent.rfind(tag, 0) == std::string::npos)
				return {};

			const size_t lineEnd = aShaderContent.find("\n");

			std::string type = aShaderContent.substr(tag.size(), lineEnd - tag.size() - 1);
			aShaderContent.erase(0, lineEnd + 1);

			return GetShaderTypeFromTypeStr(type.c_str());
		}
	}

	bool ShaderImporter::Import(ImportContext& ctx) const
	{
		glslang::InitializeProcess();

		ArtifactWriter& writer = ctx.Out();

		ShaderType type;

		const std::filesystem::path path = ctx.Meta()->SourcePath();
		std::ifstream file(path, std::ios::binary);
		std::string content = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

		if (path.extension().string() == ".glsl")
		{
			type = StripShaderType(content);
		} 
		else
		{
			type = GetShaderTypeFromExtension(path.extension().string().c_str());
		}

		writer.Write(&type, sizeof(ShaderType));

		EShLanguage lang;
		switch (type)
		{
		case ShaderType::VertexShader:	 lang = EShLangVertex; break;
		case ShaderType::ComputeShader:  lang = EShLangCompute; break;
		case ShaderType::GeometryShader: lang = EShLangGeometry; break;
		case ShaderType::PixelShader:	 lang = EShLangFragment; break;
		}

		glslang::TShader shader(lang);
		const char* src = content.c_str();
		shader.setStrings(&src, 1);
		shader.setEnvInput(glslang::EShSourceGlsl, lang, glslang::EShClientOpenGL, 450);
		shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

		shader.setAutoMapLocations(true);
		shader.setAutoMapBindings(true);

		const EShMessages msgs = EShMsgSpvRules;
		if (!shader.parse(GetDefaultResources(), 450, false, msgs))
		{
			LOG_ERROR("Shader parse failed: " + path.generic_string() + "\n" + shader.getInfoLog());
			return false;
		}

		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(msgs))
		{
			LOG_ERROR("Shader link failed: " + path.generic_string() + "\n" + program.getInfoLog());
			return false;
		}

		if (!program.mapIO())
		{
			LOG_ERROR("Shader IO mapping failed: " + path.generic_string() + "\n" + program.getInfoLog());
			return false;
		}

		{ // Reflect the shader.
			//program.buildReflection(EShReflectionDefault | EShReflectionSeparateBuffers | EShReflectionAllBlockVariables);

			//for (int i = 0; i < program.getNumUniformBlocks(); i++)
			//{
			//	const glslang::TObjectReflection& b = program.getUniformBlock(i);
			//	auto naem = b.name;
			//	auto size = b.size;
			//}

			//for (int i = 0; i < program.getNumUniformVariables(); i++)
			//{
			//	const glslang::TObjectReflection& t = program.getUniform(i);
			//	auto d = t.arrayStride;
			//}
		}

		std::vector<unsigned int> spirv;
		glslang::GlslangToSpv(*program.getIntermediate(lang), spirv);
		
		const Size contentSize = spirv.size();
		writer.Write(&contentSize, sizeof(Size));

		writer.Write(spirv.data(), contentSize * sizeof(unsigned int));

		return true;
	}
}