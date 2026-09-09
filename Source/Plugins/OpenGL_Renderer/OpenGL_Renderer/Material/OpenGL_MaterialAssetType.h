#pragma once

#include "Assets/AssetTypes/IAssetType.h"

namespace Eclipse::Assets
{
	class OpenGL_MaterialAssetType : public IAssetType
	{
	public:
		OpenGL_MaterialAssetType();

	public:
		ImportedData Import(const AssetMeta& file) override;
		ProcessedData Process(const ImportedData& file) override;
		void Serialize(BinaryWriter& writer, const ProcessedData& data) override;
		void Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data) override;
		void LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data) override;
	};
}