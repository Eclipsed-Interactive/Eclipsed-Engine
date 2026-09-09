#include "TextureInspector.h"

#include "Assets/Core/SupportedAssets.h"
#include "ImGui/ImGui.h"

//#include "Editor/Views/Types/SpriteEditor.h"
#include "Assets/Core/AssetDatabase.h"
#ifdef ECL_EDITOR

#include "Assets/Metadata/Data/TextureMeta.h"
#include "Core/MainSingleton.h"
namespace Eclipse::Editor
{
	bool TextureInspector::CanInspect(const InspectableTarget& target)
	{
		if (!std::holds_alternative<AssetTarget>(target)) return false;

		AssetTarget asset = std::get<AssetTarget>(target);
		return Assets::GetAssetTypeFromExtension(asset.extension().string()) == Assets::AssetType::Texture;
	}

	void TextureInspector::Draw(const InspectableTarget& target)
	{
		AssetTarget asset = std::get<AssetTarget>(target);

		Assets::AssetDatabase& database = MainSingleton::GetInstance<Assets::AssetDatabase>();
		Assets::GUID guid = database.GetGUIDFromFullPath(asset);

		ImGui::Text("The sprite editor needs to be re implemented.");

		if (ImGui::Button("Open in sprite editor"))
		{
			/*SpriteEditor::SetTexture(guid);*/

		}
	}
}

#endif