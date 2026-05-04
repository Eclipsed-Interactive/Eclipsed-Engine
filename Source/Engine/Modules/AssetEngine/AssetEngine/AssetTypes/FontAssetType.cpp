#include "FontAssetType.h"

namespace Eclipse::Assets
{
    ImportedData FontAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData FontAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();

    }

    void FontAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset FontAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}