#pragma once

#include <unordered_map>
#include <string>

namespace Eclipse::Assets
{
	enum class AssetType
	{
		Unknown,

		Texture,
		Material,
		Shader,
		AudioClip,
		Font,
		Prefab,
	};

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
		
		{".vglsl",  AssetType::Shader},
		{".pglsl",  AssetType::Shader},
		{".glsl",  AssetType::Shader},
		
		{".mp3",  AssetType::AudioClip},
		
		{".ttf",  AssetType::Font},
		{".otf",  AssetType::Font},
	
		{".eprf",  AssetType::Prefab},
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