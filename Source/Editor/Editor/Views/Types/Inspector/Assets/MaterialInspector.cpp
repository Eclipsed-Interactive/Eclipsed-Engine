#include "MaterialInspector.h"

#include "ImGui/ImGui.h"

#include "cereal/archives/json.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <cfloat>

#include "Editor/Common/DragAndDrop.h"

#include <Core/MainSingleton.h>
#include <Core/PathManager.h>

namespace Eclipse::Editor
{
	bool MaterialInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Material;
		return false;
	}

	void MaterialInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);
	}
}
