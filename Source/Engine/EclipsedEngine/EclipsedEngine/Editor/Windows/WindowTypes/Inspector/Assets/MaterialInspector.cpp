#include "MaterialInspector.h"

#include "AssetEngine/SupportedAssets.h"
#include "ImGui/ImGui.h"

#include "EclipsedEngine/Editor/Windows/EditorField.h"

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

		Assets::MaterialData data;
		{ // Read material
			std::ifstream in(asset);
			cereal::JSONInputArchive ar(in);
			ar(data);
		}

		bool wasChanged = EditorFieldDrawer<Assets::Material, Assets::MaterialData>::DrawEditor(data);
		if (wasChanged)
		{
			std::ofstream out(asset);
			cereal::JSONOutputArchive ar(out);
			ar(data);
		}
	}
}