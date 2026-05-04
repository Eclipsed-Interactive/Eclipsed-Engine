#include "TextureAssetType.h"

namespace Eclipse::Assets
{
    ImportedData TextureAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData TextureAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();

    }

    void TextureAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset TextureAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}