#include "AssetManager.h"

#include "Core/MainSingleton.h"
#include "AssetTypes/IAssetType.h"
#include "Assets/IO/BinaryWriter.h"

#include "AssetTypeRegistry.h"

#include "Core/PathManager.h"

#include "Assets/Assets/AudioAsset.h"
#include "Assets/Assets/FontAsset.h"
#include "Assets/Assets/MaterialAsset.h"
#include "Assets/Assets/PrefabAsset.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Assets/Shader/PixelShaderAsset.h"
#include "Assets/Assets/Shader/VertexShaderAsset.h"

#include "Assets/IO/BinaryReader.h"

#include "Core/AssetDatabase.h"
#include "AssetLoader.h"
#include "AssetImporter.h"
#include "AssetFactory.h"

#include "AssetDeletionQueue.h"
#include "Assets.Core.hpp"

namespace Eclipse::Assets
{
	bool AssetManager::IsFileReady(const std::string& path)
	{
		for (int i = 0; i < 5; i++)
		{
			std::fstream file(path, std::ios::in | std::ios::binary);

			if (file.is_open())
				return true;

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		return false;
	}



	void AssetManager::EndFrame()
	{
#ifdef ECL_EDITOR
		ProcessFileChanges();

		AssetDeletionQueue::ProcessPendingForDelete();
		AssetCache::ProcessReloads();
#endif

	}



	void AssetManager::Init()
	{
		MainSingleton::RegisterInstance<AssetDatabase>();
		AssetDatabase& database = MainSingleton::GetInstance<AssetDatabase>();
		database.ProcessSource(PathManager::GetEngineAssetsPath(), "Engine/");

		AssetTypeRegistry::RegisterTypes();
	}

	void AssetManager::CookAndPackageAssets()
	{
		AssetDatabase& database = MainSingleton::GetInstance<AssetDatabase>();

		std::ofstream out(PathManager::GetProjectRoot() / "Build/assets.bundle", std::ios::binary | std::ios::trunc);

		std::vector<Assets::GUID> guids;
		for (auto& [guid, file] : database.GetSources())
		{
			guids.push_back(guid);
		}

		size_t size = guids.size();
		out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

		struct Header {
			Assets::GUID guid;
			size_t size;
			size_t offset;
			AssetType type;
			char name[32];
		};

		size_t offset = sizeof(size_t) + sizeof(Header) * size;
		for (const Assets::GUID& guid : guids)
		{
			const Assets::AssetMeta& meta = database.GetProcessedFile(guid);
			const size_t fileSize = std::filesystem::file_size(meta.exportedPath);

			Header header;/* = { guid, fileSize, offset, meta.type, meta.fullPath.filename().stem().string().c_str() };*/

			header.guid = guid;
			header.size = fileSize;
			header.offset = offset;
			header.type = meta.type;

			std::string fileName = meta.fullPath.filename().stem().string();
			strncpy(header.name, fileName.c_str(), 32);
			header.name[sizeof(header.name) - 1] = '\0';

			out.write(reinterpret_cast<const char*>(&header), sizeof(Header));

			offset += fileSize;
		}

		for (const Assets::GUID& guid : guids)
		{
			const Assets::AssetMeta& meta = database.GetProcessedFile(guid);
			const size_t fileSize = std::filesystem::file_size(meta.exportedPath);

			std::ifstream file(meta.exportedPath, std::ios::binary);
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string content = buffer.str();

			out.write(content.data(), content.size());
		}
	}

	void AssetManager::AddFileChanged(const Editor::FileWatcherEvent& e)
	{
		std::lock_guard<std::mutex> lock(fileChangesMutex);
		fileChanges.push_back(e);
	}

	void AssetManager::ProcessFileChanges()
	{
		std::vector<Editor::FileWatcherEvent> localChanges;

		// SNAPSHOT (prevents all iterator/race issues)
		{
			std::lock_guard<std::mutex> lock(fileChangesMutex);
			localChanges = std::move(fileChanges);
			fileChanges.clear();
		}

		AssetDatabase& database =
			MainSingleton::GetInstance<AssetDatabase>();

		for (auto& e : localChanges)
		{
			// optional safety check (file still exists / not mid-write)
			if (!std::filesystem::exists(e.path))
				continue;

			if (GetAssetTypeFromExtension(std::filesystem::path(e.path).extension().string()) == AssetType::Unknown) continue;;

			switch ((Editor::EventType)e.action)
			{
			case Editor::EventType::Modified:
			case Editor::EventType::FileAdded:
			{
				if (std::filesystem::path(e.path).extension() == ".meta")
				{
					const AssetMeta& meta =
						database.GetMetaFromMetaPath(e.path);

					AssetImporter::ImportFile(meta);
				}
				else
				{
					const AssetMeta& meta =
						database.ProcessFile(e.path, e.root);

					AssetImporter::ImportFile(meta);
					AssetCache::MarkForReload(meta.guid);
				}
			}
			break;

			default:
				break;
			}
		}
	}

	bool AssetManager::FileWasChanged(const AssetMeta& meta)
	{
		std::filesystem::path exportedPath = PathManager::GetArtifactsPath();
		std::string guidStr = meta.guid.ToString();

		exportedPath /= guidStr.substr(0, 2);
		exportedPath /= guidStr;

		if (!std::filesystem::exists(exportedPath))
			return true;

		std::filesystem::path metaPath = meta.fullPath;
		metaPath += ".meta";

		auto sourceTime = std::filesystem::last_write_time(meta.fullPath);
		auto exportTime = std::filesystem::last_write_time(exportedPath);

		bool sourceChanged = sourceTime > exportTime;

		bool metaChanged = false;
		if (std::filesystem::exists(metaPath))
		{
			auto metaTime = std::filesystem::last_write_time(metaPath);
			metaChanged = metaTime > exportTime;
		}

		return sourceChanged || metaChanged;
	}




	template<typename T>
	inline T AssetManager::Load(const GUID& guid)
	{
		return AssetFactory::ConstructAsset<T>(AssetLoader::Load<T>(guid));
	}
	template<typename T>
	inline T AssetManager::LoadDefault(DefaultAssetType assetType)
	{
		return AssetFactory::ConstructAsset<T>(AssetLoader::LoadDefaultAsset<T>(assetType));
	}


	template ASSET_API Texture AssetManager::Load<Texture>(const GUID&);
	template ASSET_API AudioClip AssetManager::Load<AudioClip>(const GUID&);
	template ASSET_API Font AssetManager::Load<Font>(const GUID&);
	template ASSET_API Material AssetManager::Load<Material>(const GUID&);
	template ASSET_API Prefab AssetManager::Load<Prefab>(const GUID&);
	template ASSET_API PixelShader AssetManager::Load<PixelShader>(const GUID&);
	template ASSET_API VertexShader AssetManager::Load<VertexShader>(const GUID&);
	template ASSET_API Scene AssetManager::Load<Scene>(const GUID&);


	template ASSET_API Texture AssetManager::LoadDefault<Texture>(DefaultAssetType);
	template ASSET_API AudioClip AssetManager::LoadDefault<AudioClip>(DefaultAssetType);
	template ASSET_API Font AssetManager::LoadDefault<Font>(DefaultAssetType);
	template ASSET_API Material AssetManager::LoadDefault<Material>(DefaultAssetType);
	template ASSET_API Prefab AssetManager::LoadDefault<Prefab>(DefaultAssetType);
	template ASSET_API PixelShader AssetManager::LoadDefault<PixelShader>(DefaultAssetType);
	template ASSET_API VertexShader AssetManager::LoadDefault<VertexShader>(DefaultAssetType);
	template ASSET_API Scene AssetManager::LoadDefault<Scene>(DefaultAssetType);
}