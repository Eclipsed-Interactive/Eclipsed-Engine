#pragma once

#include "AssetEngine/Pipeline/Compiler/IEditorAssetCompiler.h"

namespace Eclipse::Assets
{
	class MaterialCompiler : public IEditorAssetCompiler
	{
	public:
		void Internal_Compile(const AssetMeta& meta, std::vector<unsigned char>& binaryData) override;
	};
}