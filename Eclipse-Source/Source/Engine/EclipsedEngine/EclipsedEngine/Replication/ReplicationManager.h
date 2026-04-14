#pragma once

#include <unordered_map>

#include "asio/asio.hpp"

#include "NetworkEngine/Shared/Message.h"

namespace Eclipse
{
    class SteamP2PNetworkingClient;
    class SteamP2PNetworkingServer;
    
    class GameObject;
    class Component;

    class Prefab;
}

namespace Eclipse::Replication
{
    class BaseReplicatedVariable;

    class ReplicationManager
    {
    public:
        friend class BaseReplicatedVariable;

        ReplicationManager() = default;
        ~ReplicationManager() = default;

        static void ReplicatedOnPlay();

        static void ReplicateVariable(unsigned aComponentID, unsigned aVariableID);

        static void CreateServer();
        static void CreateClient();

        static void Init();
        
        static void Start(bool isClient);
        static void Update();

        static void ClearList() { PossibleReplicatedVariableList.clear(); RealReplicatedVariableList.clear(); };

        static void EmplaceReplicatedVariable(unsigned ComponentID, BaseReplicatedVariable* Variable)
        {
            // Suppose to add componentid if it does not exist
            (*ReplicatedVariabpePtr)[ComponentID].emplace_back(Variable);
        }

        static void DeleteReplicatedComponent(unsigned aComponentID)
        {
            ReplicatedVariabpePtr->erase(aComponentID);
        }

        static void ClientConnected();
        static void ServerConnected();
        static void RelayNetworkReady();


    public:
    // Create replication messages
        static void CreateGOMessage(int aGameobjectID, NetMessage& outMessage);
        static void DeleteGOMessage(int aGameobjectID, NetMessage& outMessage);
        static void CreateComponentMessage(Eclipse::Component* aComponent, NetMessage& outMessage, bool aStartLater = false);
        static void CreatePrefabMessage(unsigned aGOID, const char* PrefabAssetID, std::vector<unsigned>, NetMessage& outMessage);
    public:
        // Direct send functions
        static void SendPrefabObject(GameObject* gameobject, Prefab& aPrefab);

    private:
        static inline std::unordered_map<unsigned, std::vector<BaseReplicatedVariable*>>* BeforeReplicatedVariableList;
    public:
        static void SetBeforeReplicatedList();
        static void SetAfterReplicatedList();

    public:

        static inline std::unordered_map<unsigned, std::vector<BaseReplicatedVariable*>> TemporaryReplicatedVariableList;

        static inline std::unordered_map<unsigned, std::vector<BaseReplicatedVariable*>> PossibleReplicatedVariableList;
        static inline std::unordered_map<unsigned, std::vector<BaseReplicatedVariable*>> RealReplicatedVariableList;

        static inline std::unordered_map<unsigned, std::vector<BaseReplicatedVariable*>>* ReplicatedVariabpePtr = &PossibleReplicatedVariableList;

        static inline SteamP2PNetworkingClient* client = nullptr;
        static inline SteamP2PNetworkingServer* server = nullptr;

        static inline bool startServer = true;
        static inline bool startClient = false;

        static inline bool startedGame = false;

        static inline asio::io_context ioContext;

        static inline std::string IP = "";
    };
}