#include "AudioClipInspector.h"

#include "AssetEngine/SupportedAssets.h"
#include "ImGui/ImGui.h"

namespace Eclipse::Editor
{
	bool AudioClipInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::AudioClip;
	}

	void AudioClipInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		ImGui::Text("This is a audio clip inspector!");
		ImGui::Text((std::string("AudioClip: ") + asset.generic_string()).c_str());
	}
}