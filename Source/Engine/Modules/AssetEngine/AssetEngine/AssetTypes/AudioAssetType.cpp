#include "AudioAssetType.h"

namespace Eclipse::Assets
{
    ImportedData AudioAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData AudioAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();
    }

    void AudioAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset AudioAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}