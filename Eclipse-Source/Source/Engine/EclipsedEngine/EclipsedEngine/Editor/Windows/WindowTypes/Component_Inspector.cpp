#include "InspectorWindow.h"

#include "EclipsedEngine/Editor/Common/DragAndDrop.h"

#include "EclipsedEngine/Components/Audio/AudioEmitter.h"
#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

namespace Eclipse
{
	//void AudioSource::DrawInspector()
	//{
	//	std::stringstream ss;
	//	ss << "##" << this;

	//	bool hasAudioClip = myAudioClip;


	//	if (Editor::DragAndDrop::BeginTarget(hasAudioClip ? myAudioClip->GetRelativePath() : "No audio clip", Utilities::FileInfo::FileType_Audio))
	//	{
	//		myAudioPath = Editor::DragAndDrop::payloadBuffer;
	//		SetAudioClip(myAudioPath->c_str());
	//	}

	//	if (!hasAudioClip) return;

	//	if (!myIsPlaying && ImGui::Button("Play"))
	//	{
	//		Play();
	//	}
	//	else if (myIsPlaying)
	//	{
	//		if (myIsPaused)
	//		{
	//			if (ImGui::Button("Start"))
	//			{
	//				Play();
	//			}
	//		}
	//		else
	//		{
	//			if (ImGui::Button("Pause"))
	//			{
	//				Pause();
	//			}
	//		}

	//		ImGui::SameLine();

	//		if (ImGui::Button("Stop"))
	//		{
	//			Stop();
	//		}
	//	}
	//}




	//void SpriteRenderer2D::DrawInspector()
	//{
	//	AssetRegistry& registry = AssetRegistry::GetInstance();

	//	ImGui::Text("Texture");
	//	ImGui::SameLine();

	//	if (Editor::DragAndDrop::BeginTarget(hasTexture ? registry.GetRegisteredAsset(sprite.GetAssetID()).path.filename().string().c_str() : "No sprite", Utilities::FileInfo::FileType_Texture))
	//	{
	//		std::string path = Editor::DragAndDrop::payloadBuffer;
	//		SetTexture(path.c_str());
	//	}

	//	ImGui::Spacing();

	//	ImGui::Text("Material");
	//	ImGui::SameLine();
	//	if (Editor::DragAndDrop::BeginTarget(hasMaterial ? registry.GetRegisteredAsset(material.GetAssetID()).path.filename().string().c_str() : "No material", Utilities::FileInfo::FileType_Material))
	//	{
	//		std::string path = Editor::DragAndDrop::payloadBuffer;
	//		SetMaterial(path.c_str());
	//	}
	//}
}