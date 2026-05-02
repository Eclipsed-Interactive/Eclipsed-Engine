#include "IEditorAssetCompiler.h"

#include "AssetEngine/SupportedAssets.h"
#include "AssetEngine/Pipeline/AssetDataBase.h"
#include "CoreEngine/MainSingleton.h"

using namespace std::filesystem;

namespace Eclipse::Assets
{
    std::vector<unsigned char> IEditorAssetCompiler::Compile(const AssetMeta& meta)
    {
        std::vector<unsigned char> binaryData;

        std::ofstream out(meta.GetArtifactPath(), std::ios::binary);
        Internal_Compile(meta, binaryData);

        return binaryData;
    }

    void IEditorAssetCompiler::AddToBinaryData(const void* data, size_t size, std::vector<unsigned char>& binaryData)
    {
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data);
        binaryData.insert(binaryData.end(), bytes, bytes + size);
    }
}