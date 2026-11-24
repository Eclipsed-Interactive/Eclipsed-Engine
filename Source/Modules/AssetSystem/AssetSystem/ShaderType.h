#pragma once

namespace Eclipse::AssetSystem
{
	enum ShaderType : unsigned char
	{
		ComputeShader,
		PixelShader,
		VertexShader,
		GeometryShader
	};

	constexpr ShaderType GetShaderTypeFromTypeStr(const char* aType)
	{
		std::string type = aType;
		if (type == "vertex") return ShaderType::VertexShader;
		else if (type == "pixel") return ShaderType::PixelShader;
		else if (type == "geometry") return ShaderType::GeometryShader;
		else if (type == "compute") return ShaderType::ComputeShader;

		throw;
	}

	constexpr ShaderType GetShaderTypeFromExtension(const char* aType)
	{
		std::string type = aType;

		if (type == ".vglsl") return ShaderType::VertexShader;
		else if (type == ".pglsl") return ShaderType::PixelShader;
		else if (type == ".gglsl") return ShaderType::GeometryShader;
		else if (type == ".cglsl") return ShaderType::ComputeShader;

		throw;
	}

	constexpr const char* ShaderTypeToString(ShaderType aType)
	{
		switch (aType)
		{
		case ComputeShader:		return "ComputeShader";
		case PixelShader:		return "PixelShader";
		case VertexShader:		return "VertexShader";
		case GeometryShader:	return "GeometryShader";
		}
	}

	constexpr const char* ShaderTypeToExtension(ShaderType aType)
	{
		switch (aType)
		{
		case ComputeShader:		return ".cglsl";
		case PixelShader:		return ".pglsl";
		case VertexShader:		return ".vglsl";
		case GeometryShader:	return ".gglsl";
		}
	}
}