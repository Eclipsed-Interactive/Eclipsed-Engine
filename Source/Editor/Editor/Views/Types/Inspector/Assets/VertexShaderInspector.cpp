#include "VertexShaderInspector.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool VertexShaderInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::VertexShader;
		return false;
	}

	void VertexShaderInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);
	}
}