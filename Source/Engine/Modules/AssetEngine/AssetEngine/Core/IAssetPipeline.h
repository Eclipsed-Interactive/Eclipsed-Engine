#pragma once

namespace Eclipse::Assets
{
    class IAssetPipeline 
    {
    public:
        virtual void Execute(const AssetMeta& meta) = 0;
    }
}