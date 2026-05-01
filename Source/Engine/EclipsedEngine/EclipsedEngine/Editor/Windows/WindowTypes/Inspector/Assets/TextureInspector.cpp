#include "TextureInspector.h"

#include "AssetEngine/SupportedTypes.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool TextureInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return GetAssetTypeFromExtension(asset.extension().string()) == AssetType::Texture;
	}

	void TextureInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a texture inspector!");
		ImGui::Text((std::string("Texture: ") + asset.generic_string()).c_str());
	}
}