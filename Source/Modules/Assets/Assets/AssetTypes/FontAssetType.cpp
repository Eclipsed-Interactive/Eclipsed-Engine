#include "FontAssetType.h"

#include "FreeType/ft2build.h"
#include "FreeType/freetype.h"

#include "Core/MainSingleton.h"
#include "Helper/TextManager.h"

#include "OpenGL/glad/glad.h"

namespace Eclipse::Assets
{
	FontAssetType::FontAssetType()
	{
		defaultAssetsGuids[DefaultAssetType::TEXT_FONT].FromString("9261db8212af380f50e58543eb7d2ffe");
	}

	ImportedData FontAssetType::Import(const AssetMeta& file)
    {
        ImportedFont font;

        std::ifstream in(file.fullPath, std::ios::binary | std::ios::ate);
        size_t size = in.tellg();

        in.seekg(0);

        font.Data.resize(size);
        in.read(reinterpret_cast<char*>(font.Data.data()), size);

        return font;
    }

    ProcessedData FontAssetType::Process(const ImportedData& file)
    {
        return file;
    }

    void FontAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {
        if (!writer.IsOpen()) return;

        const ImportedFont& _data = std::get<ImportedFont>(data);

        const size_t size = _data.Data.size();
        writer.Write(DATA_SIZE_PAIR(size));
        writer.Write(_data.Data.data(), _data.Data.size());
    }

    void FontAssetType::Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
    {
	//	size_t size;
	//	reader.Read(DATA_SIZE_PAIR(size));

	//	std::vector<unsigned char> storedData(size);
	//	reader.Read(storedData.data(), size);

	//	FT_Library freetype = MainSingleton::GetInstance<TextManager>().freeType;

	//	FT_Face face;
	//	if (FT_New_Memory_Face(freetype, reinterpret_cast<const FT_Byte*>(storedData.data()), size, 0, &face))
	//	{
	///*		LOG_ERROR("Failed to load font correctly");
	//		return nullptr;*/
	//	}

	//	//for (int fontSize : fontData->setFontSizes)
	//	{
	//		FT_Set_Pixel_Sizes(face, 0, 200);

	//		InternalFont font;
	//		font.myCharTexture.resize(128);
	//		for (int i = 0; i < 128; i++)
	//		{
	//			char characterChar = (char)i;
	//			if (FT_Load_Char(face, characterChar, FT_LOAD_RENDER))
	//			{
	//				//LOG_ERROR((("Failed to load character: ") + std::string(&characterChar)).c_str());
	//				continue;
	//			}

	//			if (face->glyph->bitmap.buffer == 0)
	//				continue;

	//			unsigned texture;
	//			glGenTextures(1, &texture);
	//			glBindTexture(GL_TEXTURE_2D, texture);
	//			glTexImage2D(
	//				GL_TEXTURE_2D,
	//				0,
	//				GL_RED,
	//				face->glyph->bitmap.width,
	//				face->glyph->bitmap.rows,
	//				0,
	//				GL_RED,
	//				GL_UNSIGNED_BYTE,
	//				face->glyph->bitmap.buffer
	//			);

	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//			Character character = {
	//				texture,
	//				Math::Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//				Math::Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//				face->glyph->advance.x
	//			};

	//			font.myCharTexture[characterChar] = character;
	//		}

	//		font.maxCharacterHeight = (float)(face->max_advance_height >> 6) * 0.01f;
	//		
	//		
	//		FontData* _data = reinterpret_cast<FontData*>(data);
	//		_data->font = font;
	//		_data->guid = meta.guid;
	//	}

	//	FT_Done_Face(face);
    }

	void FontAssetType::LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data)
	{
		data->guid = meta.guid;
		reader.SetRead(meta.offset);

		Load(reader, meta, data);
	}
}