#include "TextureInspector.h"

#include "ImGui/ImGui.h"
#ifdef ECL_EDITOR

#include "Core/MainSingleton.h"
namespace Eclipse::Editor
{
	bool TextureInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Texture;
		return false;
	}

	void TextureInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);
	}
}

#endif