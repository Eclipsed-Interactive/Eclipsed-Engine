#pragma once

#include <filesystem>
#include <unordered_map>

#include <Core/GUID.h>

#include "SourceMount.hpp"
#include "AssetSystem.Core.hpp"

namespace Eclipse::AssetSystem
{
	std::string VirtualPath(const std::string& aScheme, const std::filesystem::path& aFullPath);
	std::string VirtualPath(const SourceMount& aMount, const std::filesystem::path& aFullPath);

	class ASSETSYSTEM_API SourceRegistry final
	{
	public:
		static void Mount(const char* aScheme, const std::filesystem::path& aSourceLocation, const std::filesystem::path& aArtifactLocation);

		static SourceMount* TryGetMount(const char* aScheme);

		static void RefreshAll();

		static void Refresh(const char* aScheme);
		static void Refresh(SourceMount& aMount);

		static bool NeedsImporting(const SourceMount& aMount, const AssetMeta& aMeta);

		static const std::filesystem::path& GetExportPath(const SourceMount& aMount);
		static std::filesystem::path GetArtifactPath(const SourceMount& aMount, const AssetMeta& aMeta);

	private:
		static std::vector<SourceMount> sources;
	};
}