#pragma once

#include "AssetEngine/Runtime/Managers/AssetManager.h"
#include "AssetEngine/SupportedTypes.h"
#include <string>
#include <filesystem>
#include <fstream>

#include "CoreEngine/Exception/EclipseException.h"

namespace Eclipse
{
	class Resources {
	public:
		static void Init();

		template<typename T>
		static T Get(const std::string& aGuid);

		template<typename T>
		static bool CleanUp(const std::string& aGuid);

		static void TryCleanUp(const std::string& aGuid);
		template<typename T>
		static T GetDefault();

	private:
		static VertexShaderManager vertexShaderManager;
		static PixelShaderManager pixelShaderManager;
		static MaterialManager materialManager;
		static TextureManager textureManager;
		static PrefabManager prefabManager;
		static AudioClipManager audioClipManager;
	};

	template<typename T>
	inline T Resources::Get(const std::string& aGuid)
	{
		if (aGuid.empty()) return {};

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

		return {};
	}

	template<typename T>
	inline bool Resources::CleanUp(const std::string& aGuid)
	{
		if constexpr (std::is_same<T, Texture>::value) return textureManager.CleanUp(aGuid);
		else if constexpr (std::is_same<T, VertexShader>::value) return vertexShaderManager.CleanUp(aGuid);
		else if constexpr (std::is_same<T, PixelShader>::value) return pixelShaderManager.CleanUp(aGuid);
		else if constexpr (std::is_same<T, Material>::value) return materialManager.CleanUp(aGuid);
		else if constexpr (std::is_same<T, Prefab>::value) return prefabManager.CleanUp(aGuid);

		return {};
	}

	template<typename T>
	inline T Resources::GetDefault()
	{
		if constexpr (std::is_same<T, Material>::value)
		{
			std::string guid = "c88768d2848748424f797159699b3d5c";

			std::filesystem::path exportFolderPath = PathManager::GetArtifactsPath() / guid.substr(0, 2) / guid;
			std::ifstream in(exportFolderPath, std::ios::binary);

			AssetType type = AssetType::Unknown;
			in.read(reinterpret_cast<char*>(&type), sizeof(int));

			if (!in.is_open()) return {};

			return materialManager.Get(guid, in);
		}
	}
}