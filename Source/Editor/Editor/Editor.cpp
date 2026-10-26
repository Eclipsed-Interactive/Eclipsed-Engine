#include "Editor.h"

#include "Views/ViewManager.h"
#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui.h"

#include "MainMenuBar.h"
#include "ProjectManager.h"

#include "Assets/AssetImporter.h"
#include "Core/PathManager.h"

#include "EditorLayout.h"
#include "EditorUI.h"

#include "Editor/Game/GameModuleManager.h"
#include "Reflection/Reflection.hpp"

#include "Core/Standard/Serialization/Serialization.hpp"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "Editor/Common/EditorActions.h"

namespace Eclipse::Editor
{
	void Editor::EarlyInit()
	{
		ProjectManager::LoadOrSelectProject();
	}

	void Editor::Init(void* imguiCtx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)imguiCtx);

		Assets::AssetImporter::ImportAssets(PathManager::GetEngineAssetsPath(), "Engine Assets");

		EditorUI::Init();
		EditorLayout::Init();

		GameModuleManager::TryCompileAndLoad();

		SceneManager::LoadLastActive();
	}

	void Editor::Shutdown()
	{
		EditorLayout::Shutdown();
	}

	void Editor::BeginFrame()
	{
		EditorActions::Update();
	}
	
	void Editor::Render()
	{
		MainMenuBar::Draw();
		EditorLayout::Draw();
		//ViewManager::Update();


		ImGui::Begin("Scene Window");

		if (ImGui::Button("Save Scene"))
			SceneManager::SaveActive();
		if (ImGui::Button("Load Scene"))
			SceneManager::Load("Default");

		ImGui::End();

	}
	
	void Editor::EndFrame()
	{
		EditorLayout::EndFrame();
	}
}