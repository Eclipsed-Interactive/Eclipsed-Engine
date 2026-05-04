#include "MaterialAssetType.h"

namespace Eclipse::Assets
{
    ImportedData MaterialAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData MaterialAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();

    }

    void MaterialAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset MaterialAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}