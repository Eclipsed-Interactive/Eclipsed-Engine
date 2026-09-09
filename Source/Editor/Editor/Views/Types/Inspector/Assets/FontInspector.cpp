#include "FontInspector.h"

#include "Assets/Core/SupportedAssets.h"
#include "ImGui/ImGui.h"

#include "Assets/Core/AssetDatabase.h"
#include "Core/MainSingleton.h"

namespace Eclipse::Editor
{
	bool FontInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);


		return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Font;
	}

	void FontInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
		Assets::GUID guid = database.GetGUIDFromFullPath(asset);

		ImGui::Text("This is a font inspector!");
		ImGui::Text((std::string("Font: ") + asset.generic_string()).c_str());
		ImGui::Text((std::string("GUID: ") + guid.ToString()).c_str());
	}
}