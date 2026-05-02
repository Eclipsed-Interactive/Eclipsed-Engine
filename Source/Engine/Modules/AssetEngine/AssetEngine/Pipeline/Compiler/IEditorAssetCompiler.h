#pragma once

#include "AssetEngine/Pipeline/Metadata/AssetMeta.h"
#include <string>
#include <vector>

namespace Eclipse::Assets
{
	class IEditorAssetCompiler
	{
	public:
		virtual ~IEditorAssetCompiler() = default;

		virtual void Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData) = 0;
		std::vector<unsigned char> Compile(const AssetMeta& meta);

		void AddToBinaryData(const void* data, size_t size, std::vector<unsigned char>& binaryData);
	};
}