#include "TextureImporter.h"

#include "AssetSystem/Import/ImportContext.h"
#include "Core/Helpers/STB_Helper.h"

#include "AssetSystem/MetaData/TextureMeta.h"

namespace Eclipse::AssetSystem
{
	bool TextureImporter::Import(ImportContext& ctx) const
	{
		ArtifactWriter& writer = ctx.Out();

		int width = 0, height = 0, channels = 0;
		unsigned char* pixels = nullptr;
		pixels = STB_Helper::Load_Texture_STB(ctx.GetPath().generic_string().c_str(), width, height, channels, 0, true);
		if (!pixels) return false;

		writer.Write(&width, sizeof(width));
		writer.Write(&height, sizeof(height));
		writer.Write(&channels, sizeof(channels));
		writer.Write(pixels, width * height * channels);

		const TextureMeta* meta = reinterpret_cast<const TextureMeta*>(ctx.Meta());
		if (meta)
		{
			const int size = (int)meta->spriteRects.size();
			writer.Write(&size, sizeof(int));
			for (const Rect& rect : meta->spriteRects)
			{
				const Math::RectSizePos& sizePos = rect.rect;
				writer.Write(&sizePos, sizeof(Math::RectSizePos));
			}
		}

		{ // write a pre view of the texture
			int newWidth = 0, newHeight = 0;
			if (width >= height)
			{
				newWidth = 128.f;
				newHeight = (float)height / (float)width * 128.f;
			}
			else
			{
				newHeight = 128.f;
				newWidth = (float)width / (float)height * 128.f;
			}

			unsigned char* resizedPixels = STB_Helper::Resize_STB(pixels, width, height, newWidth, newHeight, channels);

			const std::string guid = ctx.Meta()->GUID().ToString();

			const std::filesystem::path previewPath = PathManager::GetArtifactsPath() / "Previews" / (guid + ".preview");
			std::filesystem::create_directories(previewPath.parent_path());

			std::ofstream file(previewPath, std::ios::binary);

			file.write(reinterpret_cast<const char*>(&newWidth), sizeof(int));
			file.write(reinterpret_cast<const char*>(&newHeight), sizeof(int));
			file.write(reinterpret_cast<const char*>(&channels), sizeof(int));
			file.write(reinterpret_cast<const char*>(resizedPixels), newHeight * newWidth * channels);

			STB_Helper::FreeData_STB(resizedPixels);
		}


		return true;
	}
}