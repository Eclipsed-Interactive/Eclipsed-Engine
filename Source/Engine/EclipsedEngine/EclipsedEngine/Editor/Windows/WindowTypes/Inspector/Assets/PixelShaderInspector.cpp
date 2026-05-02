#include "PixelShaderInspector.h"

#include "AssetEngine/SupportedAssets.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool PixelShaderInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::PixelShader;
	}

	void PixelShaderInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a pixel shader inspector!");
		ImGui::Text((std::string("Pixel Shader: ") + asset.generic_string()).c_str());
	}
}