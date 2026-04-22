#include "MaterialLoader.h"

#include "AssetEngine/Resources.h"

namespace Eclipse
{
	MaterialData* MaterialLoader::Load(std::ifstream& in) const
	{
		MaterialData* data = new MaterialData;

		size_t textureGuid = 0;
		size_t pixelShaderGuid = 0;
		size_t vertexShaderGuid = 0;



		in.read(reinterpret_cast<char*>(&textureGuid), sizeof(size_t));
		in.read(reinterpret_cast<char*>(&pixelShaderGuid), sizeof(size_t));
		in.read(reinterpret_cast<char*>(&vertexShaderGuid), sizeof(size_t));

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