#pragma once

#include "AssetEngine/Runtime/Managers/AssetManager.h"
#include "AssetEngine/SupportedTypes.h"
#include <string>
#include <filesystem>
#include <fstream>

namespace Eclipse
{
	class Resources
	{
	public:
		static void Init();

		template <typename T>
		static T Get(const size_t& aGuid);

		template <typename T>
		static bool CleanUp(const size_t& aGuid);

		static void TryCleanUp(const size_t& aGuid);

		static Material GetDefaultUIMaterial();
		static Material GetDefaultSpriteMaterial();
		static Font GetDefaultFont();

	private:
		static VertexShaderManager vertexShaderManager;
		static PixelShaderManager pixelShaderManager;
		static MaterialManager materialManager;
		static TextureManager textureManager;
		static PrefabManager prefabManager;
		static FontManager fontManager;
		static AudioClipManager audioClipManager;
	};

    template <typename T>
    inline T Resources::Get(const size_t& aGuid)
    {
        if (aGuid == 0) return {};

        std::filesystem::path exportFolderPath = PathManager::GetArtifactsPath() / aGuid.substr(0, 2) / aGuid;
        if (!std::filesystem::exists(exportFolderPath)) return {};

		std::ifstream in(exportFolderPath, std::ios::binary);

		AssetType type = AssetType::Unknown;
		in.read(reinterpret_cast<char*>(&type), sizeof(int));

		if constexpr (std::is_same<T, Texture>::value) return textureManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, VertexShader>::value) return vertexShaderManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, PixelShader>::value) return pixelShaderManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, Material>::value) return materialManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, AudioClip>::value) return audioClipManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, Prefab>::value) return prefabManager.Get(aGuid, in);
		else if constexpr (std::is_same<T, Font>::value) return fontManager.Get(aGuid, in);

		return {};
	}

    template <typename T>
    inline bool Resources::CleanUp(const size_t& aGuid)
    {
        if constexpr (std::is_same<T, Texture>::value) return textureManager.CleanUp(aGuid);
        else if constexpr (std::is_same<T, VertexShader>::value) return vertexShaderManager.CleanUp(aGuid);
        else if constexpr (std::is_same<T, PixelShader>::value) return pixelShaderManager.CleanUp(aGuid);
        else if constexpr (std::is_same<T, Material>::value) return materialManager.CleanUp(aGuid);
        else if constexpr (std::is_same<T, Prefab>::value) return prefabManager.CleanUp(aGuid);

		return {};
	}
}
