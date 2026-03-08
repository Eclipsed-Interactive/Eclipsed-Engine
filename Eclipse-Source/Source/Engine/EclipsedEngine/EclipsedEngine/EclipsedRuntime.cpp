#include "EclipsedRuntime.h"

#include "CoreEngine/MainSingleton.h"
#include "EclipsedEngine/Input/Input.h"

#include "PhysicsEngine/PhysicsEngine.h"
#include "EntityEngine/ComponentManager.h"
#include "GraphicsEngine/OpenGL/OpenGLGraphicsAPI.h"

#include "AssetEngine/Resources.h"

#include "CoreEngine/PathManager.h"

#include "Scenes/SceneManager.h"
#include "AudioEngine/AudioManager.h"

#include <fstream>

#include "EclipsedEngine/Components/Transform2D.h"
#include "EclipsedEngine/Components/Rendering/SpriteRenderer2D.h"

#include "Replication/ReplicationManager.h"

#include "NetworkEngine/Client/Client.h"
#include "NetworkEngine/Server/Server.h"
#include "Steam/SteamGeneral.h"

#include "CoreEngine/Settings/GraphicsSettings.h"

#include "EclipsedEngine/Editor/PhysicsDebugDrawer.h"

#include "EclipsedEngine/Components/ComponentForcelink.h"

#include <Input/Input.h>

#include "NetworkEngine/Server/SteamP2PNetworkingServer.h"
#include "NetworkEngine/Client/SteamP2PNetworkingClient.h"

#include "steamsdk/isteamnetworkingsockets.h"

namespace Eclipse
{
	template Transform2D* ComponentManager::GetComponent<Transform2D>(GameObjectID);
	template SpriteRenderer2D* ComponentManager::GetComponent<SpriteRenderer2D>(GameObjectID);

#ifdef ECLIPSED_EDITOR
	void EclipsedRuntime::StartEngine(const std::string& path)
#else
	void EclipsedRuntime::StartEngine()
#endif
	{
		SteamGeneral::Get().Init();
		
		AudioManager::Init();

		Replication::ReplicationManager::Init();
		ComponentForcelink::LinkComponents();
		Resources::Init();

#ifdef ECLIPSED_EDITOR
		{
			const char* appData = std::getenv("APPDATA");

			std::filesystem::path path = appData;
			path /= "EclipsedEngine";

			if (!std::filesystem::exists(path))
			{
				std::filesystem::create_directories(path);
			}

			path /= "EnginePath.txt";

			std::ofstream out(path);

			std::string engineRoot = PathManager::GetEngineRoot().generic_string();
			out.write(engineRoot.c_str(), engineRoot.size());
			out.close();
		}
		
#endif
		
		engine.Init();

		SceneManager::LoadSceneData();

		//MainSingleton::RegisterInstance<EngineSettings>();
 
		GraphicsEngine::Init();
		Input::Init();

		//Settings::SettingsRegistry::SaveDefaults();

		{ // PHYSICS
			b2DebugDraw debugDraw = { 0 };

			PhysicsDebugDrawer::Init(&debugDraw);

			PhysicsEngine::Init(8, { 0.f, -9.81f }, debugDraw);
			PhysicsEngine::myBeginContactCallback = [](UserData& aUserData)
				{
					ComponentManager::BeginCollisions(aUserData.gameobject);
				};
			PhysicsEngine::myEndContactCallback = [](UserData& aUserData)
				{
					ComponentManager::EndCollisions(aUserData.gameobject);
				};
		}

		SceneManager::LoadScene(1);
	}

	void EclipsedRuntime::UpdateGame()
	{
		//TODO: Might not want to call every frame but it does now
		SteamGeneral::Get().Update();
		
		PhysicsEngine::Update();

		ComponentManager::AwakeStartComponents();

		ComponentManager::EarlyUpdateComponents();
		ComponentManager::UpdateComponents();
		ComponentManager::LateUpdateComponents();

		AudioManager::Update();

		Replication::ReplicationManager::Update();

		SteamP2PNetworkingServer::Get().Update();
		SteamP2PNetworkingClient::Get().Update();

		if (Input::GetKeyDown(Keycode::S))
			SteamP2PNetworkingServer::Get().Start();
			//Replication::ReplicationManager::Start();

		if (Input::GetKeyDown(Keycode::C))
			SteamP2PNetworkingClient::Get().Start(76561198368166721);

		if (Input::GetKeyDown(Keycode::M))
		{
			const char* testString = "hello there person";
			SteamP2PNetworkingClient::Get().Send(testString, strlen(testString), EMessageType::Garantied);
		}
	}

	void EclipsedRuntime::Render()
	{
		PhysicsEngine::DrawPhysicsObjects();
		ComponentManager::RenderComponents();
		ComponentManager::AfterRenderUpdateComponents();
		GraphicsEngine::Render();
	}

	void EclipsedRuntime::Update()
	{
		engine.Update();
		Input::Update();
		ComponentManager::EditorUpdateComponents();
	}

	void EclipsedRuntime::EndFrame()
	{
		GraphicsEngine::EndFrame();
	}
	void EclipsedRuntime::Shutdown()
	{
		MainSingleton::Destroy();
		engine.End();

		if (MainSingleton::Exists<Server>())
		{
			auto& server = MainSingleton::GetInstance<Server>();
			server.ShutDown();
		}

		if (MainSingleton::Exists<Client>())
		{
			auto& client = MainSingleton::GetInstance<Client>();
			client.ShutDown();
		}
	}

	bool EclipsedRuntime::BeginFrame()
	{
		GraphicsEngine::BeginFrame();
		int shouldCloseWindow = GraphicsEngine::ShouldWindowClose();

		return !shouldCloseWindow;
	}
}
