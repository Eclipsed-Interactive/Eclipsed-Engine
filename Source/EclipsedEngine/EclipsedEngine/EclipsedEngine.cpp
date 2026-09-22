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
#include "Input/Input.h"

namespace Eclipse
{
	void Engine::Init()
	{
		EventSystem::Trigger("Engine-Load");

		PathManager::Init();

		ComponentManager::Init();
		Assets::AssetManager::Init();
		Timer::Init();

		renderer = &Graphics::RendererManager::LoadRenderer(Graphics::RendererAPI::OpenGL);
		MainSingleton::AddInstance(renderer);

		renderer->Init();

		Graphics::CommandListManager::InitAllCommandLists();

		ImGui_Init();
	}

	void Engine::LateInit()
	{
		Input::Input::Init(renderer->CreateInput(), GetImGuiContext());
		Assets::AssetImporter::ImportAssets(PathManager::GetAssetsPath(), "Assets");

		{ // TO be removed.
			GameObject* gameobject = ComponentManager::CreateGameObject();

			auto t = gameobject->AddComponent<Transform2D>();
			Math::Vector2f scale = { 10.f, 10.f };
			t->SetScale(scale);
			t->DirtyUpdate();
			gameobject->AddComponent<SpriteRenderer2D>();
		}
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
		Input::Input::Update();

		renderer->BeginFrame();

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