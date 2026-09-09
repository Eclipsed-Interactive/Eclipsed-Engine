#ifdef ECL_EDITOR
#include "EditorActions.h"

#include "ImGui/imgui.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include "Editor/Views/Types/HierarchyView.h"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "EclipsedEngine/Scenes/SceneLoader.h"
#include "ECS/ComponentManager.h"
#include "Core/Clipboard.h"

#include "EclipsedEngine/Components/Transform2D.h"

#include "EclipsedEngine/Reflection/Reflection.h"
#include "Core/ChatGPT_Dump/Base64.hpp"

//#include "EclipsedEngine/ECS/ObjectManager.h"

#include "Core/BatchScript.h"

#include "Editor/Game/GameCompiler.h"
#include "Editor/Game/GameLoader.h"

#include "Core/PathManager.h"

namespace Eclipse::Editor
{
	void EditorActions::SaveScene()
	{
		switch (SceneManager::GetActiveSceneType())
		{
		case SceneManager::Default:
			SceneManager::SaveActiveScene();
			break;
		case SceneManager::Prefab:
			std::filesystem::path filePath = SceneManager::GetActiveScene();
			filePath.replace_filename("");
			HierarchyView::CreatePrefab(SceneManager::ActivePrefabEditSceneID, filePath);
			break;
		}
	}
	void EditorActions::Save()
	{
		if (true)
		{
			SaveScene();
		}
		else if (false)
		{
			// Save active like sprite editor
		}

	}

	void EditorActions::CopyGameObject(unsigned activeGO, rapidjson::Value& gameobjectJson, rapidjson::Document::AllocatorType& anAllocator)
	{
		rapidjson::Value componentArray(rapidjson::kArrayType);
		componentArray.SetArray();

		auto& reflectionList = Reflection::ReflectionManager::GetList();
		auto components = ComponentManager::GetComponents(activeGO);
		for (Component* pComp : components)
		{
			std::string compName = pComp->GetComponentName();

			if (compName == "Component")
				continue;

			rapidjson::Value component(rapidjson::kObjectType);

			rapidjson::Value componentVars(rapidjson::kObjectType);
			if (reflectionList.find(pComp) != reflectionList.end())
			{
				for (auto& var : reflectionList.at(pComp))
				{
					SceneLoader::WriteMember(componentVars, var, anAllocator);
				}
			}

			rapidjson::Value isReplicatedValue(rapidjson::kObjectType);
			isReplicatedValue.SetBool(pComp->IsReplicated);


			component.AddMember(rapidjson::Value(compName.c_str(), anAllocator).Move(), componentVars, anAllocator);

			if (pComp->IsReplicated)
			{
				int huiads = 89;
			}
			component.AddMember("IsReplicated", isReplicatedValue, anAllocator);

			componentArray.PushBack(component, anAllocator);
		}
		gameobjectJson.AddMember("Name", rapidjson::Value(ComponentManager::myEntityIdToEntity.at(activeGO)->GetName().c_str(), anAllocator), anAllocator);
		gameobjectJson.AddMember("Components", componentArray, anAllocator);

		rapidjson::Value childArray(rapidjson::kArrayType);
		childArray.SetArray();

		auto& activeGameObjectObject = ComponentManager::myEntityIdToEntity.at(activeGO);

		if (activeGameObjectObject->GetChildCount())
		{
			for (auto& child : ComponentManager::myEntityIdToEntity.at(activeGO)->GetChildren())
			{
				rapidjson::Value childObject(rapidjson::kObjectType);
				childObject.SetObject();

				CopyGameObject(child->GetID(), childObject, anAllocator);

				childArray.PushBack(childObject, anAllocator);
			}

			gameobjectJson.AddMember("Children", childArray, anAllocator);
		}
	}
	rapidjson::StringBuffer EditorActions::CopyObject(int aObjectID, bool aCopyToClipboard)
	{
		if (aObjectID <= 0)
			return nullptr;

		rapidjson::Document d;
		d.SetObject();

		rapidjson::Document::AllocatorType& jsonAllocator = d.GetAllocator();

		d.AddMember("CopyType", 1, jsonAllocator);

		rapidjson::Value gameObjectArrayJson(rapidjson::kArrayType);
		gameObjectArrayJson.SetArray();

		rapidjson::Value gameobjectJson(rapidjson::kObjectType);
		gameobjectJson.SetObject();

		GameObject* pickedGameobject = ComponentManager::GetGameObject(aObjectID);

		Math::Vector2f oldLocalPosition;
		float oldLocalRotation;
		Math::Vector2f oldLocalScale;
		
		Transform2D* transform = pickedGameobject->transform;
		if (transform)
		{
			// Save old transform to unparent it
			oldLocalPosition = transform->GetLocalPosition();
			oldLocalRotation = transform->GetLocalRotation();
			oldLocalScale = transform->GetLocalScale();

			// Get the global position of the copied object
			Math::Vector2f globalPosition = transform->GetPosition();
			float globalRotation = transform->GetRotation();
			Math::Vector2f globalScale = transform->GetScale();

			// Apply the global as local on the copied object. The new object will have this transform
			transform->SetPosition(globalPosition);
			transform->SetRotation(globalRotation);
			transform->SetScale(globalScale);
		}

		CopyGameObject(aObjectID, gameobjectJson, jsonAllocator);

		if (transform)
		{
			// Reset to old locals
			transform->SetPosition(oldLocalPosition);
			transform->SetRotation(oldLocalRotation);
			transform->SetScale(oldLocalScale);
		}

		gameObjectArrayJson.PushBack(gameobjectJson, jsonAllocator);
		d.AddMember("Gameobjects", gameObjectArrayJson, jsonAllocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		const char* bufferString = buffer.GetString();
		int stringLength = strlen(bufferString);

		if (aCopyToClipboard)
			ClipBoard::CopyToClipboard(bufferString, stringLength);

		return buffer;
	}

	void EditorActions::Copy()
	{
		if (true)
		{
			CopyObject(HierarchyView::CurrentGameObjectID, true);
		}
		else if (false)
		{

		}
	}

	void EditorActions::Paste()
	{
		// COmmented this out since i cant find the InternalSpawnObjectClass

		//if (true)
		//{
		//	if (!HierarchyView::CurrentGameObjectID)
		//		return;
		//	
		//	char* data = (char*)ClipBoard::GetClipboardData();
		//	if (!data)
		//		return;
		//	GameObject* newGameobject = InternalSpawnObjectClass::CreateObjectFromJsonString(data);
		//	HierarchyView::CurrentGameObjectID = newGameobject->GetID();
		//}
		//else if (false)
		//{

		//}
	}

	void EditorActions::Update()
	{
		if (ImGui::IsAnyItemActive())
			return;

		EditorActions::Action action = A_NONE;

		bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
		if (ctrl)
		{
			bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
			if (ImGui::IsKeyPressed(ImGuiKey_S, false)) action = EditorActions::Action::A_Save;

			else if (ImGui::IsKeyPressed(ImGuiKey_C, false)) action = EditorActions::Action::A_Copy;
			else if (ImGui::IsKeyPressed(ImGuiKey_V, false)) action = EditorActions::Action::A_Paste;
			else if (ImGui::IsKeyPressed(ImGuiKey_D, false)) action = EditorActions::Action::A_Duplicate;

			// Possible might want to be able to repeat this but ill leave it as no
			// also does not exist anyway yet
			else if (ImGui::IsKeyPressed(ImGuiKey_Z, false)) action = EditorActions::Action::A_Undo;
			else if (ImGui::IsKeyPressed(ImGuiKey_R, false) || (shift && ImGui::IsKeyPressed(ImGuiKey_Z))) action = EditorActions::Action::A_Redo;
		}

		ReactToAction(action);
	}

	void EditorActions::ReactToAction(EditorActions::Action anAction)
	{
		switch (anAction)
		{
		case EditorActions::Action::A_Save:
			Save();
			break;
		case EditorActions::Action::A_Copy:
			Copy();
			break;
		case EditorActions::Action::A_Paste:
			Paste();
			break;
		case EditorActions::Action::A_Duplicate:
			Copy();
			Paste();
			break;
		}
	}



	void EditorActions::GenerateGame()
	{
		//GameModuleManager::GenerateGameEditor();
	}

	void EditorActions::CompileGame()
	{
		//GameModuleManager::CompileAndLoad();
	}

	void EditorActions::OpenGameSLNX()
	{
		static BatchScript script(
			PathManager::GetProjectRoot() / "Library/Engine-Build",
			"Eclipsed-Game.slnx"
		);

		script.Run(true);
	}
}
#endif