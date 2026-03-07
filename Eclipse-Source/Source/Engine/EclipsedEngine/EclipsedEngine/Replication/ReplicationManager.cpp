#include "ReplicationManager.h"

#include "CoreEngine/Timer.h"
#include "CoreEngine/MainSingleton.h"

#include "EntityEngine/Component.h"

#include "AssetEngine/Assets/Prefab.h"

#include "NetworkEngine/Client/Client.h"
#include "NetworkEngine/Server/Server.h"
#include "EclipsedEngine/Replication/Replication.h"
#include "ReplicatedVariable.h"

#include <functional>
#include <fstream>



namespace Eclipse::Replication
{
    void ReplicationManager::ReplicatedOnPlay()
    {
        for (auto& ReplicatedVariableLists : PossibleReplicatedVariableList)
        {
            RealReplicatedVariableList.emplace(ReplicatedVariableLists);
        }

        ReplicatedVariabpePtr = &RealReplicatedVariableList;
    }

    void ReplicationManager::ReplicateVariable(unsigned aComponentID, unsigned aVariableID)
    {
        RealReplicatedVariableList.at(aComponentID)[aVariableID]->ReplicateThis(aComponentID);
    }

    void ReplicationManager::CreateServer()
    {
        server = &Eclipse::MainSingleton::RegisterInstance<Server>(false, ioContext, [](const NetMessage& aMessage) {Replication::ReplicationHelper::ServerHelp::HandleRecieve(aMessage);});
    }
    void ReplicationManager::CreateClient()
    {
        const char* ip = ReplicationManager::IP.c_str();
        client = &Eclipse::MainSingleton::RegisterInstance<Client>(false, ioContext, ip, [](const NetMessage& aMessage) {Replication::ReplicationHelper::ClientHelp::HandleRecieve(aMessage);});

        NetMessage message = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_Connect, nullptr, 0, true);

        client->Send(message, [ip]() {
            NetMessage message = NetMessage::BuildGameObjectMessage(0, MessageType::Msg_RequestSceneInfo, nullptr, 0, true);
            client->Send(message);
            });
    }

    void SetComponentReplicationManager()
    {
        ComponentManager::SetCreateComponentReplicated([](Component* aComponent)
            {
                if (!MainSingleton::Exists<Client>())
                    return;

                NetMessage message;
                Replication::ReplicationManager::CreateComponentMessage(aComponent, message);
                MainSingleton::GetInstance<Client>().Send(message);
            });

        ComponentManager::SetDestroyGameObjectReplicated([](unsigned aGameObject)
            {
                if (!MainSingleton::Exists<Client>())
                    return;

                NetMessage message;
                Replication::ReplicationManager::DeleteGOMessage(aGameObject, message);
                MainSingleton::GetInstance<Client>().Send(message);
            });

        ComponentManager::SetDeleteReplicationComponent([](unsigned aComponentID) { Replication::ReplicationManager::DeleteReplicatedComponent(aComponentID); });
        ComponentManager::SetBeforeAfterComponentConstruction(
            []() { Replication::ReplicationManager::SetBeforeReplicatedList(); },
            []() { Replication::ReplicationManager::SetAfterReplicatedList(); });
    }

    void ReplicationManager::Init()
    {
#ifdef ECLIPSED_EDITOR
        SetComponentReplicationManager();

        std::fstream stream("NetworkIp.ntwrk");
        char IpString[16];
        stream.getline(IpString, 16);

        IP = IpString;
#endif
    }

    void ReplicationManager::Start()
    {
        if (startedGame)
            return;

        if (startServer)
            CreateServer();

        if (startClient)
            CreateClient();

        ReplicatedOnPlay();

        startedGame = true;
    }

    void ReplicationManager::Update()
    {
        if (client)
            client->Update();
        if (server)
            server->Update();

        static float timer = 0.f;
        timer -= Time::GetDeltaTime();

        if (timer <= 0)
        {
            for (auto& [_, ReplicationVariableList] : RealReplicatedVariableList)
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

    void ReplicationManager::CreatePrefabMessage(unsigned aGOID, const char* PrefabAssetID, std::vector<unsigned> aComponentIDs, NetMessage& outMessage)
    {
        char Data[512];

        const int guidSize = 32;
        int componentsCount = aComponentIDs.size();
        int totalComponentPrefabsize = aComponentIDs.size() * sizeof(unsigned);
        int DataAmount = guidSize + sizeof(componentsCount) + totalComponentPrefabsize;

        int offset = 0;

        memcpy(Data, PrefabAssetID, guidSize);
        offset += guidSize;
        memcpy(Data + offset, &componentsCount, sizeof(componentsCount));
        offset += sizeof(componentsCount);
        memcpy(Data + offset, aComponentIDs.data(), totalComponentPrefabsize);
        offset += totalComponentPrefabsize;

        outMessage = NetMessage::BuildGameObjectMessage(aGOID, MessageType::Msg_InstantiatePrefab, Data, DataAmount, true, false);
    }


    void ReplicationManager::SendPrefabObject(GameObject* gameobject, Prefab& aPrefab)
    {
        std::vector<unsigned> componentIDs;
        std::vector<Component*> components = gameobject->GetComponents();

        for (auto& component : components)
            componentIDs.emplace_back(component->myInstanceComponentID);

        NetMessage message;
        Replication::ReplicationManager::CreatePrefabMessage(gameobject->GetID(), aPrefab.GetAssetID().c_str(), componentIDs, message);

        if (Eclipse::MainSingleton::Exists<Server>())
        {
            Server& server = Eclipse::MainSingleton::GetInstance<Server>();
            server.Send(message);
            return;
        }
        else if (Eclipse::MainSingleton::Exists<Client>())
        {
            Client& client = Eclipse::MainSingleton::GetInstance<Client>();
            client.Send(message);
            return;
        }
    }

    void ReplicationManager::SetBeforeReplicatedList()
    {
        BeforeReplicatedVariableList = &(*ReplicatedVariabpePtr);
        ReplicatedVariabpePtr = &TemporaryReplicatedVariableList;
    }

    void ReplicationManager::SetAfterReplicatedList()
    {
        ReplicatedVariabpePtr = &(*BeforeReplicatedVariableList);

        for (auto& [_, ReplicatedVariableList] : ReplicationManager::TemporaryReplicatedVariableList)
        {
            for (auto Var : ReplicatedVariableList)
                (*ReplicatedVariabpePtr)[Var->ConnectedComponent->myInstanceComponentID].emplace_back(Var);
        }

        ReplicationManager::TemporaryReplicatedVariableList.erase(0);

    }
}