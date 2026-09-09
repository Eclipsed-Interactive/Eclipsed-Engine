#include "EclipsedEngine.h"

#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"
#include "Core/MainSingleton.h"
#include "ImGui/imgui.h"
#include "ImGui/External/implot.h"
#include "Core/PathManager.h"
#include "Core/Timer.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetImporter.h"
#include "EclipsedEngine/Plugin/PluginManager.h"
#include "EclipsedEngine/Components/Transform2D.h"

#include "Core/EventSystem/EventSystem.h"

namespace Eclipse
{
	void Engine::Init()
	{
		EventSystem::Trigger("Engine-Load");

		PathManager::Init();
		PathManager::SetProjectPath("C:/Users/zulto/Desktop/MyFiles/Projects/Project-Nova");

		ComponentManager::Init();
		Assets::AssetManager::Init();
		Timer::Init();

		renderer = &Graphics::RendererManager::LoadRenderer(Graphics::RendererAPI::OpenGL);
		MainSingleton::AddInstance(renderer);

		renderer->Init();

		input = renderer->CreateInput();
		input->Init();

		PluginManager::Compile("C:/Users/zulto/Desktop/GamePlugin");

		//GameObject* gameobject = ComponentManager::CreateGameObject();

		//gameobject->AddComponent<Transform2D>();
		//gameobject->AddComponent<SpriteRenderer2D>();

		ImGui_Init();
	}

	void Engine::LateInit()
	{
		Assets::AssetImporter::ImportAssets(PathManager::GetAssetsPath(), "Assets");
	}



	bool Engine::ShouldClose()
	{
		return renderer->ShouldWindowClose();
	}

	void* Engine::GetImGuiContext()
	{
		return ImGui::GetCurrentContext();
	}

	void Engine::EndFrame()
	{
		renderer->EndFrame();
	}

	void Engine::BeginFrame()
	{
		Timer::Update();

		renderer->BeginFrame();
		input->Update();

		ImGui_NewFrame();
	}

	void Engine::Render()
	{
		renderer->Render();

		ImGui_Render();
	}



	void Engine::ImGui_Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();


		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.IniFilename = nullptr;

		Graphics::IRenderer& r = Graphics::RendererManager::GetRenderer();
		r.ImGui_Init(ImGui::GetCurrentContext());

		ImPlot::CreateContext();
	}

	void Engine::ImGui_NewFrame()
	{
		Graphics::IRenderer& r = Graphics::RendererManager::GetRenderer();
		r.ImGui_NewFrame();
		ImGui::NewFrame();
	}

	void Engine::ImGui_Render()
	{
		Graphics::IRenderer& r = Graphics::RendererManager::GetRenderer();
		ImGui::Render();
		r.ImGui_Render();
	}
}