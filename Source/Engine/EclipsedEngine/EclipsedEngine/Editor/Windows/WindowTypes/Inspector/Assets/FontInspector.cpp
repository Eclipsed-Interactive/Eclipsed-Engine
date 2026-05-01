#include "FontInspector.h"

#include "AssetEngine/SupportedTypes.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool FontInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return GetAssetTypeFromExtension(asset.extension().string()) == AssetType::Font;
	}

	void FontInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a font inspector!");
		ImGui::Text((std::string("Font: ") + asset.generic_string()).c_str());
	}
}