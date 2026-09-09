#include "OpenGL_MaterialAssetType.h"

#include "Assets/BinaryFormats/MaterialData.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "Assets/AssetManager.h"

#include "OpenGL/glad/glad.h"

namespace Eclipse::Assets
{
	OpenGL_MaterialAssetType::OpenGL_MaterialAssetType()
	{
		defaultAssetsGuids[DefaultAssetType::MATERIAL_2D_SPRITE].FromString("304024b18cd756152dee092f1a43738d");
		defaultAssetsGuids[DefaultAssetType::MATERIAL_UI].FromString("a4b24408a489129276bd23242d2946a8");
		defaultAssetsGuids[DefaultAssetType::MATERIAL_TEXT].FromString("8cc226a548b15b488d94a6635dd92236");
	}

	ImportedData OpenGL_MaterialAssetType::Import(const AssetMeta& file)
	{
		ImportedMaterial material;

		SerializableMaterialData matData;
		std::ifstream in(file.fullPath, std::ios::binary);

		cereal::JSONInputArchive ar(in);
		ar(matData);

		material.pixelShaderID.FromString(matData.pixelShaderGuidStr);
		material.vertexShaderID.FromString(matData.vertexShaderGuidStr);
		material.textureID.FromString(matData.textureGuidStr);
		material.color = matData.color;

		return material;
	}

	ProcessedData OpenGL_MaterialAssetType::Process(const ImportedData& file)
	{
		return ProcessedData();
	}

	void OpenGL_MaterialAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
	{
		if (!writer.IsOpen()) return;

		const ImportedMaterial& _data = std::get<ImportedMaterial>(data);

		writer.Write(DATA_SIZE_PAIR(_data.color));
		writer.Write(DATA_SIZE_PAIR(_data.pixelShaderID));
		writer.Write(DATA_SIZE_PAIR(_data.vertexShaderID));
		writer.Write(DATA_SIZE_PAIR(_data.textureID));
	}

	void OpenGL_MaterialAssetType::Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		MaterialData* _data = reinterpret_cast<MaterialData*>(data);

		reader.Read(DATA_SIZE_PAIR(_data->color));


		GUID pixelShaderID, vertexShaderID, textureID;
		reader.Read(DATA_SIZE_PAIR(pixelShaderID));
		reader.Read(DATA_SIZE_PAIR(vertexShaderID));
		reader.Read(DATA_SIZE_PAIR(textureID));

		if (!pixelShaderID.IsEmpty())
			_data->pixelShader = AssetManager::Load<PixelShader>(pixelShaderID);

		if (!vertexShaderID.IsEmpty())
			_data->vertexShader = AssetManager::Load<VertexShader>(vertexShaderID);

		if (!textureID.IsEmpty())
			_data->texture = AssetManager::Load<Texture>(textureID);

		_data->guid = meta.guid;

		if (!_data->pixelShader.IsValid())
		{
			// Fallback to error shader
		}

		if (!_data->vertexShader.IsValid())
		{
			// Fallback to error shader
		}

		_data->programID = glCreateProgram();
		glAttachShader(_data->programID, _data->vertexShader.GetProgramID());
		glAttachShader(_data->programID, _data->pixelShader.GetProgramID());
		glLinkProgram(_data->programID);
	}

	void OpenGL_MaterialAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		MaterialData* _data = reinterpret_cast<MaterialData*>(data);

		data->guid = meta.guid;
		reader.SetRead(meta.offset);

		reader.Read(DATA_SIZE_PAIR(_data->color));


		GUID pixelShaderID, vertexShaderID, textureID;
		reader.Read(DATA_SIZE_PAIR(pixelShaderID));
		reader.Read(DATA_SIZE_PAIR(vertexShaderID));
		reader.Read(DATA_SIZE_PAIR(textureID));

		if (!pixelShaderID.IsEmpty())
			_data->pixelShader = AssetManager::Load<PixelShader>(pixelShaderID);

		if (!vertexShaderID.IsEmpty())
			_data->vertexShader = AssetManager::Load<VertexShader>(vertexShaderID);

		if (!textureID.IsEmpty())
			_data->texture = AssetManager::Load<Texture>(textureID);

		_data->guid = meta.guid;


		if (!_data->pixelShader.IsValid())
		{
			// Fallback to error shader
		}

		if (!_data->vertexShader.IsValid())
		{
			// Fallback to error shader
		}

		_data->programID = glCreateProgram();
		glAttachShader(_data->programID, _data->vertexShader.GetProgramID());
		glAttachShader(_data->programID, _data->pixelShader.GetProgramID());
		glLinkProgram(_data->programID);
	}
}