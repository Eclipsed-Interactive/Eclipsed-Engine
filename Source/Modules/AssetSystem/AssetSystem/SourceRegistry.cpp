#include "SourceRegistry.h"

#include "Import/AssetImporter.h"
#include "Artifacts/LooseProvider.h"
#include "Artifacts/ArtifactRegistry.h"

#include <Core/Files/FileWatcher.h>

namespace Eclipse::AssetSystem
{
	std::vector<SourceMount> SourceRegistry::sources;

	void SourceRegistry::Mount(const char* aScheme, const std::filesystem::path& aSourceLocation, const std::filesystem::path& aArtifactLocation)
	{
		Editor::FileWatcher::SubscribeToPath(aSourceLocation,
			[scheme = std::string(aScheme)](const Editor::FileWatcherEvent& e)
			{
				SourceRegistry::Refresh(scheme.c_str());
			}
		);

		SourceMount mount;
		mount.scheme = aScheme;
		mount.sourceLocation = aSourceLocation;
		mount.artifactLocation = aArtifactLocation;

		LooseProvider* provider = new LooseProvider(aArtifactLocation);
		ArtifactRegistry::Mount(provider);
		mount.provider = provider;

		sources.push_back(std::move(mount));

		Refresh(sources.back());
	}

	SourceMount* SourceRegistry::TryGetMount(const char* aScheme)
	{
		for (SourceMount& mount : sources)
			if (mount.scheme == aScheme)
				return &mount;

		return nullptr;
	}

	bool SourceRegistry::TryGetGuid(const std::filesystem::path& aFullPath, GUID& outGuid)
	{
		for (SourceMount& mount : sources)
		{
			auto rel = std::filesystem::relative(aFullPath, mount.sourceLocation);
			if (rel.empty() || *rel.begin() == "..") 
				continue;

			auto it = mount.pathToGuid.find(VirtualPath(mount, aFullPath));
			if (it == mount.pathToGuid.end()) 
				return false;
			
			outGuid = it->second;

			return true;
		}
		return false;
	}

	bool SourceRegistry::TryGetFullPath(GUID aGuid, std::filesystem::path& outPath)
	{
		for (const auto& mount : sources)
		{
			auto it = mount.metas.find(aGuid);
			if (it == mount.metas.end() ) continue;

			outPath = std::filesystem::absolute(it->second->SourcePath());
			return true;
		}

		return false;
	}

	void SourceRegistry::RefreshAll()
	{
		for (SourceMount& mount : sources)
			Refresh(mount);
	}

	void SourceRegistry::Refresh(const char* aScheme)
	{
		SourceMount* mount = TryGetMount(aScheme);
		if (!mount) return;

		Refresh(*mount);
	}

	void SourceRegistry::Refresh(SourceMount& aMount)
	{
		using namespace std::filesystem;

		for (auto& it : recursive_directory_iterator(aMount.sourceLocation))
		{
			if (!it.is_regular_file()) continue;
			if (it.path().extension() == ".meta") continue;

			const path assetPath = it.path();

			if (relative(assetPath, aMount.sourceLocation).generic_string()[0] == '_') continue;

			AssetMeta* meta = AssetMeta::CreateEmpty(assetPath);
			meta->Load(aMount, assetPath);

			const std::string guidStr = meta->GUID().ToString();
			meta->SetArtifactPath(aMount.artifactLocation / guidStr.substr(0, 2) / guidStr);

			const GUID guid = meta->GUID();
			aMount.pathToGuid[VirtualPath(aMount, assetPath)] = guid;

			if (NeedsImporting(aMount, *meta))
				AssetImporterd::Import(aMount, meta);

			auto existing = aMount.metas.find(guid);
			if (existing != aMount.metas.end()) delete existing->second;
			aMount.metas[guid] = meta;

			aMount.provider->Index().Add(guid, meta->VirtualPath());
		}
	}

	bool SourceRegistry::NeedsImporting(const SourceMount& aMount, const AssetMeta& aMeta)
	{
		std::error_code ec;
		const auto artifactTime = std::filesystem::last_write_time(aMeta.ArtifactPath(), ec);
		if (ec) return true;

		const auto sourceTime = std::filesystem::last_write_time(aMeta.SourcePath(), ec);
		if (ec) return true;

		const auto metaTime = std::filesystem::last_write_time(aMeta.GetMetaPath(aMeta.SourcePath()), ec);

		return sourceTime > artifactTime || (!ec && metaTime > artifactTime);
	}

	const std::filesystem::path& SourceRegistry::GetExportPath(const SourceMount& aMount)
	{
		return aMount.artifactLocation;
	}

	std::filesystem::path SourceRegistry::GetArtifactPath(const SourceMount& aMount, const AssetMeta& aMeta)
	{
		const std::string guidStr = aMeta.GUID().ToString();
		return GetExportPath(aMount) / guidStr.substr(0, 2) / guidStr;
	}

	std::string VirtualPath(const std::string& aScheme, const std::filesystem::path& aFullPath)
	{
		SourceMount* mount = SourceRegistry::TryGetMount(aScheme.c_str());
		if (!mount) return "";

		return VirtualPath(*mount, aFullPath);
	}

	std::string VirtualPath(const SourceMount& aMount, const std::filesystem::path& aFullPath)
	{
		return aMount.scheme + "://" + std::filesystem::relative(aFullPath, aMount.sourceLocation).generic_string();
	}
}