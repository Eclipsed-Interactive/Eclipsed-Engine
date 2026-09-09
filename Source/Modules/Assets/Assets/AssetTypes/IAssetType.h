#pragma once

#include "Assets/Core/ImportedData.h"
#include "Assets/Core/RuntimeAsset.h"
#include "Assets/Metadata/AssetMeta.h"

#include "Assets/IO/BinaryWriter.h"
#include "Assets/IO/BinaryReader.h"

#include "Assets/AssetTypes.h"

#define REQUIRES_PROCESSING public: bool NeedsProcessing() const override {return true;}

namespace Eclipse::Assets
{
    class BinaryReader;

    class IAssetType
    {
    public:
        virtual ImportedData Import(const AssetMeta& file) = 0;
        virtual ProcessedData Process(const ImportedData& file) { return file; }
        virtual void Serialize(BinaryWriter& writer, const ProcessedData& data) = 0;
        virtual void Load(BinaryReader& reader, const AssetMeta& meta, AssetData* data) = 0;
        virtual void LoadFromBinary(BinaryReader& reader, const AssetMeta& meta, AssetData* data) {}

        virtual bool NeedsProcessing() const { return false; }

    public:
        GUID GetDefaultAsset(DefaultAssetType defaultAsset) const;

    protected:
        std::unordered_map<DefaultAssetType, GUID> defaultAssetsGuids;
    };
}