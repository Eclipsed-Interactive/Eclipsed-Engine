#pragma once

#include "Core/GUID.h"

#include <unordered_map>
#include <typeindex>
#include <filesystem>
#include <fstream>

#include "AssetSystem/MetaData/Data/IAssetMeta.h"

#include "cereal/cereal.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

#include <memory>

#include "AssetSystem/AssetTypes.h"
#include "AssetSystem/AssetPath.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>


#include "AssetSystem.Core.hpp"

#define MOVE(type)								\
type() = default;								\
~type() = default;								\
												\
type(const type&) = delete;						\
type& operator=(const type&) = delete;			\
												\
type(type&&) noexcept = default;				\
type& operator=(type&&) noexcept = default;

namespace Eclipse::AssetSystem
{
	struct SourceMount;
	struct ASSETSYSTEM_API AssetMeta
	{
	public:
		using JsonWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

		AssetMeta() = default;
		virtual ~AssetMeta() = default;

		AssetMeta(const AssetMeta&) = delete;
		AssetMeta& operator=(const AssetMeta&) = delete;

		AssetMeta(AssetMeta&&) noexcept = default;
		AssetMeta& operator=(AssetMeta&&) noexcept = default;

	public:
		[[nodiscard]] Eclipse::GUID GUID() const { return guid; }
		[[nodiscard]] const std::string& Filename() const { return filename; }
		[[nodiscard]] const AssetPath& VirtualPath() const { return virtualPath; }
		[[nodiscard]] const std::filesystem::path& SourcePath() const { return sourcePath; }
		[[nodiscard]] const std::filesystem::path& ArtifactPath() const { return artifactPath; }
		[[nodiscard]] AssetType Type() const { return type; }

		void SetArtifactPath(const std::filesystem::path& anArtifactPath);

	public:
		static AssetMeta* CreateEmpty(const std::filesystem::path& aSource);
		static std::filesystem::path GetMetaPath(const std::filesystem::path& aPath);

		void Save();
		bool Load(const SourceMount& aMount, const std::filesystem::path& aSource);

	protected:
		virtual void SaveData(JsonWriter& writer) const {}
		virtual void LoadData(const rapidjson::Value& data) {}

	protected:
		Eclipse::GUID guid;
		std::string filename;
		AssetPath virtualPath;
		std::filesystem::path sourcePath;
		std::filesystem::path artifactPath;

		AssetType type = AssetType::Unknown;
	};
}