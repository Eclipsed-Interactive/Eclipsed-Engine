#include "Texture.h"

#include <AssetSystem/Assets/Data/TextureData.hpp>
#include <Renderer/RendererManager.h>

namespace Eclipse
{
	Texture::Texture(AssetSystem::AssetData* aData, GUID aGuid)
		: myGuid(aGuid), myData(reinterpret_cast<AssetSystem::TextureData*>(aData))
	{
	}

	bool Texture::IsReady() const
	{
		return myData != nullptr;
	}

	void Texture::Bind(unsigned int slot) const
	{
		Graphics::RendererManager::GetRenderer().GetDevice()->BindTexture(slot, myData->textureID);
	}

	Math::Vector2f Texture::GetDimDivOne() const
	{
		return IsReady() ? myData->dimDivOne : Math::Vector2f{ -1.f, -1.f };
	}

	Math::Vector2f Texture::GetSizeNormalized() const
	{
		return IsReady() ? myData->sizeNormalized : Math::Vector2f{ -1.f, -1.f };
	}

	int Texture::GetHeight() const
	{
		return IsReady() ? myData->height : -1.f;
	}

	int Texture::GetWidth() const 
	{
		return IsReady() ? myData->width: -1.f;
	}

	int Texture::GetChannels() const 
	{
		return IsReady() ? myData->channels : -1.f;
	}

	const std::vector<Math::RectSizePos>* Texture::GetSpriteRects() const
	{
		return IsReady() ? &myData->spriteRects : nullptr;
	}

	float Texture::GetAspectRatio() const
	{
		return IsReady() ? myData->ratio : 0.f;
	}

	Math::Vector2f Texture::GetTextureSizeNormilized() const
	{
		return IsReady() ? myData->sizeNormalized: Math::Vector2f{ -1.f, -1.f };
	}
}
