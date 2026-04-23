#pragma once

#include "AssetEngine/Editor/Importer/IEditorAssetImporter.h"
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
namespace FMOD {
	class System;
}

namespace Eclipse
{
	class AudioImporter : public IEditorAssetImporter
	{
	public:
		void Export(const size_t& guid, std::ofstream& outStream, const std::filesystem::path& aPath) override;
	};
}