#include "MaterialLoader.h"

#include "AssetEngine/Resources.h"

namespace Eclipse
{
	MaterialData* MaterialLoader::Load(std::ifstream& in) const
	{
		MaterialData* data = new MaterialData;

		std::string textureGuid = "";
		std::string pixelShaderGuid = "";
		std::string vertexShaderGuid = "";

		int textureNameSize = 0;
		int pixelShaderNameSize = 0;
		int vertexShaderNameSize = 0;

		in.read(reinterpret_cast<char*>(&textureNameSize), sizeof(int));
		in.read(reinterpret_cast<char*>(&pixelShaderNameSize), sizeof(int));
		in.read(reinterpret_cast<char*>(&vertexShaderNameSize), sizeof(int));

		textureGuid.resize(textureNameSize);
		pixelShaderGuid.resize(pixelShaderNameSize);
		vertexShaderGuid.resize(vertexShaderNameSize);

		in.read(textureGuid.data(), textureNameSize);
		in.read(pixelShaderGuid.data(), pixelShaderNameSize);
		in.read(vertexShaderGuid.data(), vertexShaderNameSize);

		data->texture = Resources::Get<Texture>(textureGuid);
		data->pixelShader = Resources::Get<PixelShader>(pixelShaderGuid);
		data->vertexShader = Resources::Get<VertexShader>(vertexShaderGuid);

		if (data->pixelShader.IsValid() && data->vertexShader.IsValid())
		{
			data->programID = glCreateProgram();
			glAttachShader(data->programID, data->vertexShader.GetProgramID());
			glAttachShader(data->programID, data->pixelShader.GetProgramID());
			glLinkProgram(data->programID);
		}

		return data;
	}
}