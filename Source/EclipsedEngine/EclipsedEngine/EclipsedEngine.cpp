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

#include "Physics/PhysicsEngine.h"

#include <Core/Profiling/ProfilerMacros.hpp>

#include <Core/Settings/SettingsRegistry.h>

namespace Eclipse
{
	Graphics::IRenderer* Engine::renderer = nullptr;
	Audio::IAudioBackend* Engine::audio = nullptr;

	Graphics::CommandListManager Engine::myCommandListManager ;

	void Engine::Init()
	{
		PROFILE_SCOPED;
		Settings::SettingsRegistry::LoadAll();

		MainSingleton::RegisterInstance<TextManager>();

		EventSystem::Trigger("Engine-Load");

		PathManager::Init();

		ComponentManager::Init();
		Assets::AssetManager::Init();
		Timer::Init();

		{
			// PHYSICS
			b2DebugDraw debugDraw = { 0 };

			//PhysicsDebugDrawer::Init(&debugDraw);

			PhysicsEngine::Init(8, { 0.f, -9.82f }, debugDraw);
			PhysicsEngine::myBeginContactCallback = [](UserData& aUserData, UserData& aUserData1)
				{
					ComponentManager::BeginCollisions(aUserData.gameobject, aUserData1.gameobject);
				};
			PhysicsEngine::myEndContactCallback = [](UserData& aUserData, UserData& aUserData1)
				{
					ComponentManager::EndCollisions(aUserData.gameobject, aUserData1.gameobject);
				};


			PhysicsEngine::myBeginTriggerCallback = [](UserData& aUserData, UserData& aUserData1)
				{
					ComponentManager::BeginTrigger(aUserData.gameobject, aUserData1.gameobject);
				};

			PhysicsEngine::myEndTriggerCallback = [](UserData& aUserData, UserData& aUserData1)
				{
					ComponentManager::EndTrigger(aUserData.gameobject, aUserData1.gameobject);
				};
		}

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

	void Engine::UpdateGame()
	{
		PROFILE_SCOPED;
		ComponentManager::AwakeStartComponents();

		PhysicsEngine::Update();

		ComponentManager::UpdateComponents();
	}


	void Engine::LateInit()
	{
		PROFILE_SCOPED;
		Input::Input::Init(renderer->CreateInput(), GetImGuiContext());
	}



	bool Engine::ShouldClose()
	{
		return renderer->ShouldWindowClose();
	}

	void Engine::Shutdown()
	{
		EventSystem::Trigger("Engine-Shutdown");
		Settings::SettingsRegistry::SaveAll();
	}

	void* Engine::GetImGuiContext()
	{
		return ImGui::GetCurrentContext();
	}

	void Engine::EndFrame()
	{
		renderer->EndFrame();

		myCommandListManager.ResetAllCommandLists();

		Assets::AssetManager::EndFrame();
	}

	void Engine::Update()
	{
		if (audio)
		{
			audio->Update();
		}

		auto device = Graphics::RendererManager::GetRenderer().GetDevice();
		device->BindFrameBuffer(1);

		ComponentManager::EditorUpdateComponents();
		ComponentManager::EditorLateUpdateComponents();

		ComponentManager::EarlyUpdateComponents();
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
		PROFILE_SCOPED;

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