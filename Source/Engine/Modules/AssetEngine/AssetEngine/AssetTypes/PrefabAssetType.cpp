#include "PrefabAssetType.h"

namespace Eclipse::Assets
{
    ImportedData PrefabAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData PrefabAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();

    }

    void PrefabAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset PrefabAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}