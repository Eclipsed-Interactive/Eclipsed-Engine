#include "PrefabInspector.h"

#include "AssetEngine/SupportedTypes.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool PrefabInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return GetAssetTypeFromExtension(asset.extension().string()) == AssetType::Prefab;
	}

	void PrefabInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a prefab inspector!");
		ImGui::Text((std::string("Prefab: ") + asset.generic_string()).c_str());
	}
}