//#pragma once
//
//#include <filesystem>
//#include "AssetSystem/MetaData/AssetMeta.h"
//
//namespace Eclipse::AssetSystem
//{
//	class MetaSerializer
//	{
//	public:
//		static bool MetaFileExists(const std::filesystem::path& path);
//		static AssetMeta LoadOrCreateMeta(const std::filesystem::path& assetPath);
//
//	private:
//		static AssetMeta GetMetaType(const std::filesystem::path& assetPath);
//
//		static AssetMeta CreateMeta(const std::filesystem::path& assetPath);
//		static AssetMeta LoadMeta(const std::filesystem::path& assetPath);
//		static void WriteMetaToFile(const std::filesystem::path& assetPath, const AssetMeta& assetMeta);
//
//	public:
//		static std::filesystem::path GetMetaPath(const std::filesystem::path& assetPath);
//	};
//}