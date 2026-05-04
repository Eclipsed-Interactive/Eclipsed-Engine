#include "ShaderAssetType.h"

namespace Eclipse::Assets
{
    ImportedData ShaderAssetType::Import(const AssetMeta& file)
    {
        return ImportedData();
    }

    ProcessedData ShaderAssetType::Process(const ImportedData& file)
    {
        return ProcessedData();

    }

    void ShaderAssetType::Serialize(BinaryWriter& writer, const ProcessedData& data)
    {

    }

    RuntimeAsset ShaderAssetType::Load(BinaryReader& reader, const AssetMeta& meta)
    {
        return RuntimeAsset();
    }
}