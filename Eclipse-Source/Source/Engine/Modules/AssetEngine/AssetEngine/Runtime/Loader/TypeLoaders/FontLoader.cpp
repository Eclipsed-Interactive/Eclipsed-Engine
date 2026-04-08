#include "FontLoader.h"

#include <freetype.h>

#include <fstream>

#include "AssetEngine/Helper/TextManager.h"

#include "CoreEngine/Debug/DebugLogger.h"

namespace Eclipse
{
	FontData* FontLoader::Load(std::ifstream& in) const
	{
		FontData* fontData = new FontData;

		int fileSize = 0;
		in.read(reinterpret_cast<char*>(&fileSize), sizeof(int));
		
		char* data = static_cast<char*>(malloc(fileSize));
		in.read(data, fileSize);
		
		FT_Library freetype = MainSingleton::GetInstance<TextManager>().freeType;

		FT_Face face;
		if (FT_New_Memory_Face(freetype, reinterpret_cast<const FT_Byte*>(data), fileSize, 0, &face))
		{
			LOG_ERROR("Failed to load font correctly");
			return nullptr;
		}

		//for (int fontSize : fontData->setFontSizes)
		{
			FT_Set_Pixel_Sizes(face, 0, 200);

			InternalFont font;
			for (int i = 0; i < 128; i++)
			{
				char characterChar = (char)i;
				if (FT_Load_Char(face, characterChar, FT_LOAD_RENDER))
				{
					LOG_ERROR((("Failed to load character: ") + std::string(&characterChar)).c_str());
					return nullptr;
				}

				if (face->glyph->bitmap.buffer == 0)
					continue;

				unsigned texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				Character character = {
					texture,
					Math::Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					Math::Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};

				font.myCharTexture.emplace(characterChar, character);
			}

			font.maxCharHeight = (float)(face->max_advance_height >> 6) * 0.01f;
			fontData->font = font;
		}
		
		FT_Done_Face(face);

        return fontData;
	}
}
