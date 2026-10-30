#include "MaterialInspector.h"

#include "Assets/Core/SupportedAssets.h"
#include "ImGui/ImGui.h"

#include <Assets/BinaryFormats/MaterialData.h>

#include "cereal/archives/json.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <cfloat>

#include "Editor/Common/DragAndDrop.h"

#include <Core/MainSingleton.h>
#include <Core/PathManager.h>
#include <Assets/Core/AssetDatabase.h>

namespace Eclipse::Editor
{
	bool MaterialInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Material;
	}

	void MaterialInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		Assets::SerializableMaterialData data;
		{
			std::ifstream in(asset);
			cereal::JSONInputArchive ar(in);
			ar(data);
		}

		bool wasChanged = false;
		
		if (wasChanged)
		{ 
			std::ofstream out(asset);
			cereal::JSONOutputArchive ar(out);
			ar(data);
		}
	}
}
