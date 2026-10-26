#include "Editor.h"

#include "Views/ViewManager.h"
#include "Views/EditorViewRegistry.h"

#include "ImGui/imgui.h"

#include "MainMenuBar.h"
#include "ProjectManager.h"

#include "Assets/AssetImporter.h"
#include "Core/PathManager.h"

#include "EditorLayout.h"
#include "ToolBar.h"
#include "StatusBar.h"
#include "DockSpace.h"
#include "EditorUI.h"

#include "Editor/Game/GameModuleManager.h"
#include "Reflection/Reflection.hpp"

#include "Core/Standard/Serialization/Serialization.hpp"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "Editor/Common/EditorActions.h"
#include "Core/Settings/EditorSettings.h"

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

		if (Settings::EditorSettings::GetOpenLastOpenSceneOnStartup())
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
		ToolBar::Draw();
		StatusBar::Draw();
		DockSpace::Draw();
		EditorLayout::Draw();
	}

	void Editor::EndFrame()
	{
		EditorLayout::EndFrame();
	}
}