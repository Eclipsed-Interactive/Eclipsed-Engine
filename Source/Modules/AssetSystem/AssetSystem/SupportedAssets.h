#pragma once

#include <unordered_map>
#include <string>

#include "AssetSystem/AssetTypes.h"

namespace Eclipse::AssetSystem
{
	inline std::unordered_map<std::string, AssetType> extensionToAssetType =
	{
		{".png",  AssetType::Texture},
		{".jpg",  AssetType::Texture},
		{".jpeg", AssetType::Texture},
		{".bmp",  AssetType::Texture},
		{".tga",  AssetType::Texture},
		{".psd",  AssetType::Texture},
		{".gif",  AssetType::Texture},
		{".hdr",  AssetType::Texture},
		{".pic",  AssetType::Texture},
		{".pnm",  AssetType::Texture},

		{".mat",  AssetType::Material},

		//{".vglsl",  AssetType::VertexShader},
		//{".pglsl",  AssetType::PixelShader},

		{".mp3",  AssetType::Audio},
		{".wav",  AssetType::Audio},

		{".ttf",  AssetType::Font},
		{".otf",  AssetType::Font},

		{".eprf",  AssetType::Prefab},

		{".scene", AssetType::Scene}
	};

	inline AssetType GetAssetTypeFromExtension(const std::string& extension)
	{
		auto it = extensionToAssetType.find(extension);
		if (it != extensionToAssetType.end()) {
			return it->second;
		}

		return AssetType::Unknown;
	}
}