#include "AudioClipInspector.h"

#include "ImGui/ImGui.h"

#include "Core/MainSingleton.h"

namespace Eclipse::Editor
{
	bool AudioClipInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		//return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::AudioClip;
		return false;
	}

	void AudioClipInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);


	}
}