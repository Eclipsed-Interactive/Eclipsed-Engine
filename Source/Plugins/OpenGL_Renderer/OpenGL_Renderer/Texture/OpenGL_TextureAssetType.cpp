#include "OpenGL_TextureAssetType.h"

#include "OpenGL/glad/glad.h"
#include "Core/Helpers/STB_Helper.h"

#include "Core/EventSystem/EventSystem.h"

#include "Core/Math/RectSizePos.h"

#include "Assets/Metadata/Data/TextureMeta.h"

namespace Eclipse::Assets
{
	OpenGL_TextureAssetType::OpenGL_TextureAssetType()
	{
		//defaultAssetsGuids[DefaultAssetType::TEXTURE_ERROR].FromString("cf50f44ea7fb40ed07c66d1190024581");
		//defaultAssetsGuids[DefaultAssetType::TEXTURE_DEFAULT].FromString("77cbf21c126e6ab274908f75c436065b");
	}

	ImportedData OpenGL_TextureAssetType::Import(const AssetMeta& file)
	{
		EventSystem::Trigger("Texture_Imported", file);


		ImportedTexture data;

		unsigned char* pixelData;
		pixelData = STB_Helper::Load_Texture_STB(file.fullPath.generic_string().c_str(), data.width, data.height, data.channels, true);

		const size_t size = data.width * data.height * data.channels;
		data.Data.resize(size);
		memcpy(data.Data.data(), pixelData, size);
		STB_Helper::FreeData_STB(pixelData);

		const TextureMeta* textureMeta = file.GetMetaComponent<TextureMeta>();

		if (textureMeta)
		{
			data.spriteRects.resize(textureMeta->spriteRects.size());

			for (int i = 0; i < textureMeta->spriteRects.size(); i++)
				data.spriteRects[i] = textureMeta->spriteRects[i].rect;
		}


		return data;
	}

	ProcessedData OpenGL_TextureAssetType::Process(const ImportedData& file)
	{
		return file;
	}

	void OpenGL_TextureAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
	{
		if (!writer.IsOpen()) return;

		const ImportedTexture& _data = std::get<ImportedTexture>(data);

		writer.Write(DATA_SIZE_PAIR(_data.width));
		writer.Write(DATA_SIZE_PAIR(_data.height));
		writer.Write(DATA_SIZE_PAIR(_data.channels));
		writer.Write(_data.Data.data(), _data.Data.size());

		int size = _data.spriteRects.size();
		writer.Write(DATA_SIZE_PAIR(size));
		writer.Write(_data.spriteRects.data(), size * sizeof(Math::RectSizePos));
	}

	void OpenGL_TextureAssetType::Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		TextureData* _data = reinterpret_cast<TextureData*>(data);

		reader.Read(DATA_SIZE_PAIR(_data->width));
		reader.Read(DATA_SIZE_PAIR(_data->height));
		reader.Read(DATA_SIZE_PAIR(_data->channels));

		std::vector<unsigned char> pixelData(_data->width * _data->height * _data->channels);
		reader.Read(pixelData.data(), pixelData.size());

		_data->dimDivOne.X = 1.f / static_cast<float>(_data->width);
		_data->dimDivOne.Y = 1.f / static_cast<float>(_data->height);
		_data->sizeNormalized = Math::Vector2f{ 1.f, static_cast<float>(_data->height) / _data->width };
		_data->guid = meta.guid;

		glGenTextures(1, &_data->textureID);
		glBindTexture(GL_TEXTURE_2D, _data->textureID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = GL_RGB;

		switch (_data->channels)
		{
		case 1: format = GL_RED;  break;
		case 2: format = GL_RG;   break;
		case 3: format = GL_RGB;  break;
		case 4: format = GL_RGBA; break;

		default:
			assert(false && "Unsupported channel count");
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, _data->width, _data->height, 0, format, GL_UNSIGNED_BYTE, pixelData.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		int size = 0;
		reader.Read(DATA_SIZE_PAIR(size));

		_data->spriteRects.resize(size);

		reader.Read(_data->spriteRects.data(), size * sizeof(Math::RectSizePos));
	}

	void OpenGL_TextureAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		reader.SetRead(meta.offset);

		Load(reader, meta, data);
	}
}