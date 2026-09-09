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
#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

#include "Core/EventSystem/EventSystem.h"

#include "Renderer/RenderCommands/CommandList.h"

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

		Graphics::CommandListManager::InitAllCommandLists();

		//PluginManager::Compile("C:/Users/zulto/Desktop/GamePlugin");

		GameObject* gameobject = ComponentManager::CreateGameObject();

		auto t = gameobject->AddComponent<Transform2D>();
		Math::Vector2f scale = { 100.f, 30.f };
		t->SetScale(scale);
		t->DirtyUpdate();
		gameobject->AddComponent<SpriteRenderer2D>();

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

		Graphics::CommandListManager::ResetAllCommandLists();

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		//device->BindFrameBuffer({0,1});
	}

	void Engine::Update()
	{
		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->BindFrameBuffer(1);

		ComponentManager::AwakeStartComponents();

		ComponentManager::EarlyUpdateComponents();
		ComponentManager::UpdateComponents();
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
		ComponentManager::RenderComponents();

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->SetViewport({ 1280, 720 });

		device->BindFrameBuffer(1);
		Graphics::CommandListManager::ExecuteAllCommandLists();
		

		device->BindFrameBuffer(0);
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