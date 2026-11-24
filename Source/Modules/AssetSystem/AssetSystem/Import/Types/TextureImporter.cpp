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
				//const std::string& name = rect.name;
				//const int nameSize = (int)name.size();
				const Math::RectSizePos& sizePos = rect.rect;

				//writer.Write(&nameSize, sizeof(int));
				//writer.Write(name.c_str(), nameSize);
				writer.Write(&sizePos, sizeof(Math::RectSizePos));
			}
		}

		STB_Helper::FreeData_STB(pixels);

		return true;
	}
}