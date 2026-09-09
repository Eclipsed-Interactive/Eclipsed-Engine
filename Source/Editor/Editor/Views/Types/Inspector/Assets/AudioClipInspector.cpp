#include "AudioClipInspector.h"

#include "Assets/Core/SupportedAssets.h"
#include "ImGui/ImGui.h"

#include "Assets/Core/AssetDatabase.h"
#include "Assets/Metadata/Data/AudioMeta.h"
#include "Core/MainSingleton.h"

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

		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
		Assets::GUID guid = database.GetGUIDFromFullPath(asset);

		Assets::AssetMeta& meta = database.GetProcessedFile(guid);
		Assets::AudioMeta* audioMeta = meta.GetMetaComponent<Assets::AudioMeta>();

		ImGui::Text(meta.guid.ToString().c_str());
	}
}