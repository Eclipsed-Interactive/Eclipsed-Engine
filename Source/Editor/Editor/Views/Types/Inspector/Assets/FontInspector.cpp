#include "FontInspector.h"

#include "ImGui/ImGui.h"

#include "Core/MainSingleton.h"

namespace Eclipse::Editor
{
	bool FontInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);


		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Font;
		return false;
	}

	void FontInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

	}
}