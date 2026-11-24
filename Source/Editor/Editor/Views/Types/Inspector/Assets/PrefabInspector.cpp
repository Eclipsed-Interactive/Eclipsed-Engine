#include "PrefabInspector.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool PrefabInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Prefab;
		return false;
	}

	void PrefabInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);
	}
}