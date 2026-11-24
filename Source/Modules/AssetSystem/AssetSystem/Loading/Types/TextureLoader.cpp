#include "TextureLoader.h"

#include <AssetSystem/Assets/Data/TextureData.hpp>
#include <Core/Standard/Core/Types.hpp>
#include "ArtifactHeader.h"

#include <Renderer/RendererManager.h>

namespace Eclipse::AssetSystem
{
	bool TextureLoader::Load(ArtifactReader& aReader, AssetData* aData) const
	{
		TextureData* tex = static_cast<TextureData*>(aData);

		if (!aReader.Read(&tex->width, sizeof(int))) return false;
		if (!aReader.Read(&tex->height, sizeof(int))) return false;
		if (!aReader.Read(&tex->channels, sizeof(int))) return false;

		const size_t pixelSize = (size_t)tex->width * tex->height * tex->channels;
		const std::span<const Byte> pixels = { reinterpret_cast<const Byte*>(aReader.Current()), pixelSize};
		
		aReader.Skip(pixelSize);

		int rects = 0;
		if (!aReader.Read(&rects, sizeof(int))) return false;
		if (rects > 0)
		{
			tex->spriteRects.resize(rects);
			for (int i = 0; i < rects; i++)
			{
				if(!aReader.Read(&tex->spriteRects[i], sizeof(Math::RectSizePos))) 
					return false;
			}
		}

		Graphics::IGraphicsDevice* device = Graphics::RendererManager::GetRenderer().GetDevice();
		Graphics::TextureDescription desc;
		desc.channels = tex->channels;
		desc.width = tex->width;
		desc.height= tex->height;

		tex->textureID = device->CreateTexture(pixels, desc);

		return true;
	}
}