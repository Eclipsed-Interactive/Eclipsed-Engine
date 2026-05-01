#include "VertexShaderInspector.h"

#include "AssetEngine/SupportedTypes.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool VertexShaderInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return GetAssetTypeFromExtension(asset.extension().string()) == AssetType::VertexShader;
	}

	void VertexShaderInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a vertex shader inspector!");
		ImGui::Text((std::string("Vertex Shader: ") + asset.generic_string()).c_str());
	}
}