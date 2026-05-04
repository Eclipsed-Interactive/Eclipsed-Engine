#pragma once

#include "AssetEngine/Core/ImportedData.h"
#include "AssetEngine/Core/RuntimeAsset.h"
#include "AssetEngine/Core/ProcessedData.h"
#include "AssetEngine/Metadata/AssetMeta.h"

namespace Eclipse::Assets
{
    class BinaryWriter;
    class BinaryReader;

    class IAssetType
    {
    public:
        virtual ImportedData Import(const AssetMeta& file) = 0;
        virtual ProcessedData Process(const ImportedData& file) = 0;
        virtual void Serialize(BinaryWriter& writer, const ProcessedData& data) = 0;
        virtual RuntimeAsset Load(BinaryReader& reader, const AssetMeta& meta) = 0;
    };
}