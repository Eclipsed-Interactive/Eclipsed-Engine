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

#include "Steam/SteamGeneral.h"

#include "CoreEngine/Settings/GraphicsSettings.h"

#include "EclipsedEngine/Editor/PhysicsDebugDrawer.h"

#include "EclipsedEngine/Components/ComponentForcelink.h"

#include "AssetEngine/Helper/TextManager.h"
#include "NetworkEngine/Client/SteamP2PNetworkingClient.h"
#include "NetworkEngine/Server/SteamP2PNetworkingServer.h"

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
#ifndef ECLIPSED_EDITOR
        SteamGeneral::Get().Init();
#endif

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
        MainSingleton::RegisterInstance<TextManager>();
        
        GraphicsEngine::InitSpecifiedAPI<OpenGLGraphicsEngine>();
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->Init();
        Input::Init();

        //Settings::SettingsRegistry::SaveDefaults();

        {
            // PHYSICS
            b2DebugDraw debugDraw = {0};

            PhysicsDebugDrawer::Init(&debugDraw);

            PhysicsEngine::Init(8, {0.f, -9.81f}, debugDraw);
            PhysicsEngine::myBeginContactCallback = [](UserData& aUserData)
            {
                ComponentManager::BeginCollisions(aUserData.gameobject);
            };
            PhysicsEngine::myEndContactCallback = [](UserData& aUserData)
            {
                ComponentManager::EndCollisions(aUserData.gameobject);
            };
        }

        SceneManager::LoadScene(0);
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
    }

    void SortComponents()
    {
        // const std::vector<Component*>& AllComponents = ComponentManager::GetAllComponents();
        //
        // std::vector<int> ComponentsIndecies;
        // ComponentManager::GetAllComponentsOfTypePtr<BaseUI>(ComponentsIndecies);
        //
        // std::vector<BaseUI*> UiComponents;
        // for (auto& component : ComponentsIndecies)
        // 	UiComponents.emplace_back(reinterpret_cast<BaseUI*>(AllComponents[component]));
        //       
        // std::sort(UiComponentsIndecies.begin(), UiComponentsIndecies.end(), [](BaseUI* aBaseUI0, BaseUI* aBaseUI1)
        // {
        // 	return aBaseUI0->GetZIndex() < aBaseUI1->GetZIndex();
        // });
    }

    void EclipsedRuntime::Render()
    {
        SortComponents();

        PhysicsEngine::DrawPhysicsObjects();
        ComponentManager::RenderComponents();
        ComponentManager::AfterRenderUpdateComponents();
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->Render();
    }

    void EclipsedRuntime::Update()
    {
        engine.Update();
        Input::Update();

        
#ifndef ECLIPSED_EDITOR
            Math::Vector2f mousePos = Input::GetMousePos();
        
            const Math::Vector2i& resolution = Settings::GraphicsSettings::GetResolution();

            float mousePosNormalizedX = mousePos.x / resolution.x;
            float mousePosNormalizedY = mousePos.y / resolution.y;

            Input::SetGamePosition({mousePosNormalizedX, 1 - mousePosNormalizedY});
#endif

            ComponentManager::EditorUpdateComponents();
    }

    void EclipsedRuntime::EndFrame()
    {
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->EndFrame();
    }

    void EclipsedRuntime::Shutdown()
    {
        MainSingleton::Destroy();
        engine.End();

        if (MainSingleton::Exists<SteamP2PNetworkingServer>())
        {
            auto& server = MainSingleton::GetInstance<SteamP2PNetworkingServer>();
            //server.ShutDown();
        }

        if (MainSingleton::Exists<SteamP2PNetworkingClient>())
        {
            auto& client = MainSingleton::GetInstance<SteamP2PNetworkingClient>();
            //client.ShutDown();
        }
    }

    bool EclipsedRuntime::BeginFrame()
    {
        GraphicsEngine::Get<OpenGLGraphicsEngine>()->BeginFrame();
        int shouldCloseWindow = GraphicsEngine::Get<OpenGLGraphicsEngine>()->ShouldWindowClose();

        return !shouldCloseWindow;
    }
}
