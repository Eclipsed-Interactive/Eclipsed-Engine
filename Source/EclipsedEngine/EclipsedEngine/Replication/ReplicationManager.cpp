#ifdef ECLIPSED_NETWORKING

#include "ReplicationManager.h"

#include "Core/Timer.h"
#include "Core/MainSingleton.h"

#include "ECS/Component.h"

#include "Assets/Assets/PrefabAsset.h"

#include "Networking/Client/SteamP2PNetworkingClient.h"
#include "Networking/Server/SteamP2PNetworkingServer.h"

#include "EclipsedEngine/Replication/Replication.h"
#include "ReplicatedVariable.h"

#include <functional>
#include <fstream>

#include "Steam/SteamGeneral.h"
#include "ECS/GameObject.h"

namespace Eclipse::Replication
{
    void ReplicationManager::ReplicatedOnPlay()
    {
        for (auto& ReplicatedVariableLists : BeforePlayReplicatedVariables)
        {
             if (!ReplicatedVariableLists.second[0]->OnComponent->IsReplicated)
                 continue;

            OnPlayReplicatedVariables.emplace(ReplicatedVariableLists);
        }

        CurrentReplicatedVariabpePtr = &OnPlayReplicatedVariables;
    }

    void ReplicationManager::ReplicateVariable(unsigned aComponentID, unsigned aVariableID)
    {
        OnPlayReplicatedVariables.at(aComponentID)[aVariableID]->ReplicateThis(aComponentID);
    }

    void ReplicationManager::SteamNetorkingReady()
    {
        if (server)
            ServerConnected();
        if (client)
            ClientConnected();
    }

    void ReplicationManager::ServerConnected()
    {
    }

    void ReplicationManager::ClientConnected()
    {
        NetMessage message = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_RequestSceneInfo, nullptr, 0, true);
        client->Send(message);
    }

    void ReplicationManager::CreateServer()
    {
        server = &MainSingleton::RegisterInstance<SteamP2PNetworkingServer>([](const NetMessage& aMessage) { Replication::ReplicationHelper::ServerHelp::HandleRecieve(aMessage); });
        server->Start();
    }

    void ReplicationManager::CreateClient()
    {
        client = &MainSingleton::RegisterInstance<SteamP2PNetworkingClient>([](const NetMessage& aMessage) { Replication::ReplicationHelper::ClientHelp::HandleRecieve(aMessage); });
        client->Start(SteamGeneral::OthersteamID);
    }

    void SetComponentReplicationManager()
    {
        ComponentManager::SetCreateComponentReplicated([](Component* aComponent)
        {
            if (!MainSingleton::Exists<SteamP2PNetworkingClient>())
                return;

            NetMessage message;
            Replication::ReplicationManager::CreateComponentMessage(aComponent, message);
            MainSingleton::GetInstance<SteamP2PNetworkingClient>().Send(message);
        });

        ComponentManager::SetDestroyGameObjectReplicated([](unsigned aGameObject)
        {
            if (!MainSingleton::Exists<SteamP2PNetworkingClient>())
                return;

            NetMessage message;
            Replication::ReplicationManager::DeleteGOMessage(aGameObject, message);
            MainSingleton::GetInstance<SteamP2PNetworkingClient>().Send(message);
        });

        ComponentManager::SetDeleteReplicationComponent([](unsigned aComponentID) { Replication::ReplicationManager::DeleteReplicatedComponent(aComponentID); });
        ComponentManager::SetBeforeAfterComponentConstruction(
            []() { Replication::ReplicationManager::SetBeforeReplicatedList(); },
            []() { Replication::ReplicationManager::SetAfterReplicatedList(); });
    }

    void ReplicationManager::Init()
    {
        SetComponentReplicationManager();

        // #ifdef ECL_EDITOR
        //
        //         std::fstream stream("NetworkIp.ntwrk");
        //         char IpString[16];
        //         stream.getline(IpString, 16);
        //
        //         IP = IpString;
        // #endif
    }

    void ReplicationManager::Start(bool isClient)
    {
        if (startedGame)
            return;

        if (!isClient)
            CreateServer();

        if (isClient)
            CreateClient();

        ReplicatedOnPlay();

        //ComponentManager::FindObjectByName("Spawner")->GetComponent<Eclipse::PlayerSpawner>()->StartGame();

        startedGame = true;
    }

    void ReplicationManager::Update()
    {
        if (client)
            client->Update();
        if (server)
            server->Update();

        static float timer = 0.f;
        //timer -= Time::GetDeltaTime();

        if (timer <= 0)
        {
            for (auto& [_, ReplicationVariableList] : OnPlayReplicatedVariables)
            {
                for (int i = 0; i < ReplicationVariableList.size(); i++)
                {
                    auto& Variable = ReplicationVariableList[i];
                    if (!Variable->ManualVariableSending)
                        Variable->ReplicateThis(i);
                }
            }

            timer = 0.005f;
        }
    }

    void ReplicationManager::CloseConnection(const char* aReason)
    {
        if (server)
            server->CloseConnection(aReason);
        else if (client)
            client->CloseConnection(aReason);
    }

    // This might be stupid if one thinks about it but if you only want the object then do that
    void ReplicationManager::CreateGOMessage(int aGameobjectID, NetMessage& outMessage)
    {
        outMessage = NetMessage::BuildGameObjectMessage(aGameobjectID, MessageType::Msg_CreateObject, &aGameobjectID, 0, true);
    }

    void ReplicationManager::DeleteGOMessage(int aGameobjectID, NetMessage& outMessage)
    {
        outMessage = NetMessage::BuildGameObjectMessage(aGameobjectID, MessageType::Msg_DeleteObject, &aGameobjectID, 0, true);
    }

    void ReplicationManager::CreateComponentMessage(Component* aComponent, NetMessage& outMessage, bool aStartLater)
    {
        char Data[512];

        const char* ComponentName = aComponent->GetComponentName();

        int LengthOfComponentName = strlen(ComponentName);

        memcpy(Data, &aComponent->myInstanceComponentID, sizeof(int));
        memcpy(Data + sizeof(int), &LengthOfComponentName, sizeof(int));
        memcpy(Data + sizeof(int) + sizeof(int), ComponentName, LengthOfComponentName);

        int DataAmount = LengthOfComponentName + sizeof(int) + sizeof(int);

        outMessage = NetMessage::BuildGameObjectMessage(aComponent->gameObject->GetID(), MessageType::Msg_AddComponent, Data, DataAmount, true, aStartLater);
    }

    void ReplicationManager::CreatePrefabMessage(unsigned aGOID, const Assets::GUID& PrefabAssetID, std::vector<unsigned> aComponentIDs, NetMessage& outMessage)
    {
        char Data[512];

        const std::string guidStr = PrefabAssetID.ToString();
        const int guidSize = guidStr.size();
        int componentsCount = aComponentIDs.size();
        int totalComponentPrefabsize = aComponentIDs.size() * sizeof(unsigned);
        int DataAmount = guidSize + sizeof(componentsCount) + totalComponentPrefabsize;

        int offset = 0;

        memcpy(Data, guidStr.c_str(), guidSize);
        offset += guidSize;
        memcpy(Data + offset, &componentsCount, sizeof(componentsCount));
        offset += sizeof(componentsCount);
        memcpy(Data + offset, aComponentIDs.data(), totalComponentPrefabsize);
        offset += totalComponentPrefabsize;

        outMessage = NetMessage::BuildGameObjectMessage(aGOID, MessageType::Msg_InstantiatePrefab, Data, DataAmount, true, false);
    }

    void BuildComponentVector(GameObject* aGameobject, std::vector<unsigned>& aComponentIDs)
    {
        std::vector<Component*> childComponent = aGameobject->GetAllComponents();

        for (auto& component : childComponent)
            if (component->IsReplicated)
                aComponentIDs.emplace_back(component->myInstanceComponentID);

        for (auto& child : aGameobject->GetChildren())
            BuildComponentVector(child, aComponentIDs);
    }

    void ReplicationManager::SendPrefabObject(GameObject* gameobject, Assets::Prefab& aPrefab)
    {
        std::vector<unsigned> componentIDs;
        BuildComponentVector(gameobject, componentIDs);

        NetMessage message;
        CreatePrefabMessage(gameobject->GetID(), aPrefab.GetAssetID(), componentIDs, message);

        
        if (server)
            server->Send(message);
        else if (client)
            client->Send(message);
    }

    void ReplicationManager::SetBeforeReplicatedList()
    {
        BeforeReplicatedVariableList = &(*CurrentReplicatedVariabpePtr);
        CurrentReplicatedVariabpePtr = &TemporaryReplicatedVariableList;
    }

    void ReplicationManager::SetAfterReplicatedList()
    {
        CurrentReplicatedVariabpePtr = &(*BeforeReplicatedVariableList);

        for (auto& [_, ReplicatedVariableList] : ReplicationManager::TemporaryReplicatedVariableList)
        {
            for (auto Var : ReplicatedVariableList)
                (*CurrentReplicatedVariabpePtr)[Var->OnComponent->myInstanceComponentID].emplace_back(Var);
        }

        ReplicationManager::TemporaryReplicatedVariableList.erase(0);
    }
}

#endif