#include "SceneAssetType.h"

#include "Core/Helpers/STB_Helper.h"

#include "Core/EventSystem/EventSystem.h"

namespace Eclipse::Assets
{
	ImportedData SceneAssetType::Import(const AssetMeta& file)
	{
		EventSystem::Trigger("Scene_Imported", file);

		ImportedScene data;

		std::ifstream in(file.fullPath);
		std::stringstream buffer;
		buffer << in.rdbuf();

		data.source = buffer.str();
		data.name = file.fileName;

		return data;
	}

	ProcessedData SceneAssetType::Process(const ImportedData& file)
	{
		return file;
	}

	void SceneAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
	{
		if (!writer.IsOpen()) return;

		const ImportedScene& _data = std::get<ImportedScene>(data);

		const size_t nameSize = _data.name.size();
		writer.Write(DATA_SIZE_PAIR(nameSize));
		writer.Write(_data.name.data(), nameSize);

		const size_t blobSize = _data.source.size();
		writer.Write(DATA_SIZE_PAIR(blobSize));
		writer.Write(_data.source.data(), blobSize);
	}

	void SceneAssetType::Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		SceneData* _data = reinterpret_cast<SceneData*>(data);

		_data->guid = meta.guid;

		size_t blobSize = 0;
		size_t nameSize = 0;
		reader.Read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));

		_data->name.resize(nameSize);
		reader.Read(_data->name.data(), nameSize);

		reader.Read(reinterpret_cast<char*>(&blobSize), sizeof(size_t));

		_data->sourceBlob.resize(blobSize);
		reader.Read(_data->sourceBlob.data(), blobSize);
	}

	void SceneAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		SceneData* _data = reinterpret_cast<SceneData*>(data);

		data->guid = meta.guid;
		reader.SetRead(meta.offset);

		size_t nameSize = 0;
		reader.Read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));

		_data->name.resize(nameSize);
		reader.Read(_data->name.data(), nameSize);

		size_t blobSize = 0;
		reader.Read(reinterpret_cast<char*>(&blobSize), sizeof(size_t));

		_data->sourceBlob.resize(blobSize);
		reader.Read(_data->sourceBlob.data(), blobSize);
	}
}