#pragma once

#include <filesystem>
#include <unordered_map>

#include <AssetSystem/MetaData/AssetMeta.h>
#include <Core/GUID.h>
#include <AssetSystem/Index.h>

namespace Eclipse::AssetSystem
{
	class LooseProvider;
	struct SourceMount
	{
		SourceMount() = default;
		SourceMount(const SourceMount&) = delete;
		SourceMount& operator=(const SourceMount&) = delete;
		~SourceMount() { for (auto& [guid, meta] : metas) delete meta; }

		SourceMount(SourceMount&& o) noexcept { *this = std::move(o); }
		SourceMount& operator=(SourceMount&& o) noexcept
		{
			scheme = std::move(o.scheme);
			sourceLocation = std::move(o.sourceLocation);
			artifactLocation = std::move(o.artifactLocation);
			metas = std::move(o.metas);
			o.metas.clear();
			pathToGuid = std::move(o.pathToGuid);
			provider = o.provider;
			o.provider = nullptr;
			return *this;
		}

		std::string scheme;
		std::filesystem::path sourceLocation;
		std::filesystem::path artifactLocation;

		std::unordered_map<GUID, AssetMeta*, GUIDHash> metas;
		std::unordered_map<std::string, GUID> pathToGuid;

		LooseProvider* provider = nullptr;
	};
}