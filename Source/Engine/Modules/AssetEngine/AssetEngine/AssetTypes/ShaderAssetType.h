#pragma once

#include "IAssetType.h"

namespace Eclipse::Assets
{
    class ShaderAssetType : public IAssetType
    {
    public:
        ImportedData Import(const AssetMeta& file) override;
        ProcessedData Process(const ImportedData& file) override;
        void Serialize(BinaryWriter& writer, const ProcessedData& data) override;
        RuntimeAsset Load(BinaryReader& reader, const AssetMeta& meta) override;
    };
}