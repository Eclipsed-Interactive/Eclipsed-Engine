#include "EclipsedEngine.h"

#include "Renderer/RenderCommands/CommandList.h"
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
#include "EclipsedEngine/Components/Rendering/ParticleSystem2D.h"
#include "EclipsedEngine/Components/UI/RectTransform.h"
#include "EclipsedEngine/Components/UI/TextRenderer.h"
#include "EclipsedEngine/Components/UI/Canvas.h"
#include "EclipsedEngine/Components/Rendering/Camera.h"

#include "Core/EventSystem/EventSystem.h"

#include "Input/Input.h"

#include "Audio/AudioPluginLoader.h"
#include "Assets/Helper/TextManager.h"

namespace Eclipse
{
	void Engine::Init()
	{
		MainSingleton::RegisterInstance<TextManager>();

		EventSystem::Trigger("Engine-Load");

		PathManager::Init();

		ComponentManager::Init();
		Assets::AssetManager::Init();
		Timer::Init();

		renderer = &Graphics::RendererManager::LoadRenderer(Graphics::RendererAPI::OpenGL);

		auto audioPath = PathManager::GetEngineRoot() / "Plugins" / "Eclipsed.Audio_FMOD.dll";
		Audio::AudioError error = Audio::AudioManager::LoadBackend(audioPath);
		if (error == Audio::AudioError::Succeded)
		{
			audio = Audio::AudioManager::GetBackend();
			audio->Initialize();
		}

		MainSingleton::AddInstance(renderer);

		renderer->Init();

		myCommandListManager.InitAllCommandLists();
		MainSingleton::AddInstance(myCommandListManager);

		ImGui_Init();

#ifdef ECL_EDITOR
		Assets::AssetImporter::ImportAssets(PathManager::GetAssetsPath(), "Assets");
#else
		// IMPORT BUNDLE
#endif
	}

	void Engine::LateInit()
	{
		Input::Input::Init(renderer->CreateInput(), GetImGuiContext());


		//{
		//	GameObject* gameobject = ComponentManager::CreateGameObject();
		//	auto t = gameobject->AddComponent<Transform2D>();
		//	Math::Vector2f scale = { 10.f, -10.f };
		//	t->SetScale(scale);

		//	gameobject->AddComponent<ParticleSystem2D>();
		//	gameobject->AddComponent<SpriteRenderer2D>();
		//}
	}



	bool Engine::ShouldClose()
	{
		return renderer->ShouldWindowClose();
	}

	void Engine::Shutdown()
	{
		EventSystem::Trigger("Engine-Shutdown");
	}

	void* Engine::GetImGuiContext()
	{
		return ImGui::GetCurrentContext();
	}

	void Engine::EndFrame()
	{
		renderer->EndFrame();

		myCommandListManager.ResetAllCommandLists();
	}

	void Engine::Update()
	{
		if (audio)
		{
			audio->Update();
		}

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->BindFrameBuffer(1);

		ComponentManager::AwakeStartComponents();

		ComponentManager::EditorUpdateComponents();
		ComponentManager::EditorLateUpdateComponents();

		ComponentManager::EarlyUpdateComponents();



		// Toffla
		// Move this to the on play 
		ComponentManager::UpdateComponents();
	}

	void Engine::BeginFrame()
	{
		Timer::Update();
		Input::Input::Update();

		renderer->GetDebugDrawer()->Begin();
		renderer->BeginFrame();

		ImGui_NewFrame();

		Graphics::CommandListManager* commandListManager = MainSingleton::GetPointer<Graphics::CommandListManager>();
		commandListManager->GetHappenAtBeginCommandList().Execute();
	}

	void Engine::Render()
	{
		ComponentManager::RenderComponents();

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		//device->SetViewport({ 1280, 720 });

		//device->BindFrameBuffer(1);
		//Graphics::CommandListManager::ExecuteAllCommandLists();

		device->BindFrameBuffer(0);
		device->Clear();

		renderer->Render();
	}

	void Engine::LateRender()
	{
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