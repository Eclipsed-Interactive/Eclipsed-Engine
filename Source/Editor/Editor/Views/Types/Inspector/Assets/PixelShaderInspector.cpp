#include "PixelShaderInspector.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool PixelShaderInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::PixelShader;
		return false;
	}

	void PixelShaderInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);
	}
}