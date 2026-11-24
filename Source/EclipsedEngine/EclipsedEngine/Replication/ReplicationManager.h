#pragma once

#ifdef ECLIPSED_NETWORKING
#include <unordered_map>

#include "asio/asio.hpp"

#include "Networking/Shared/Message.h"

namespace Eclipse
{
    class SteamP2PNetworkingClient;
    class SteamP2PNetworkingServer;
    
    class GameObject;
    class Component;
    struct GUID;

    namespace Assets
    {
        class Prefab;
    }
}

namespace Eclipse::Replication
{
    //typedef long long unsigned ComponentNIterator;
    class BaseReplicatedVariable;

    class ReplicationManager
    {
        typedef std::vector<BaseReplicatedVariable*> ComponentReplicatedVariables;
        typedef std::unordered_map<unsigned, ComponentReplicatedVariables> ReplicationVariableMap;

    public:
        friend class BaseReplicatedVariable;

        ReplicationManager() = default;
        ~ReplicationManager() = default;

        template <typename T>
        int GetReplicationVariableIterator();

        static void ReplicatedOnPlay();

        static void ReplicateVariable(unsigned aComponentID, unsigned aVariableID);

        static void CreateServer();
        static void CreateClient();

        static void Init();
        
        static void Start(bool isClient);
        static void Update();

        static void CloseConnection(const char* aReason);

        static void ClearList() { BeforePlayReplicatedVariables.clear(); OnPlayReplicatedVariables.clear(); };

        static void EmplaceReplicatedVariable(unsigned aComponentID, unsigned& iterationID, BaseReplicatedVariable* Variable)
        {
            ComponentReplicatedVariables& replicationVariableMap = (*CurrentReplicatedVariabpePtr)[aComponentID];

            replicationVariableMap.emplace_back(Variable);

            iterationID = replicationVariableMap.size() - 1;
        }

        static void DeleteReplicatedComponent(unsigned aComponentID)
        {
            ReplicationVariableMap& replicationVariableMap = (*CurrentReplicatedVariabpePtr);

            replicationVariableMap.erase(aComponentID);
        }

        static void ClientConnected();
        static void ServerConnected();
        static void SteamNetorkingReady();


    public:
        // Create replication messages
        static void CreateGOMessage(int aGameobjectID, NetMessage& outMessage);
        static void DeleteGOMessage(int aGameobjectID, NetMessage& outMessage);
        static void CreateComponentMessage(Eclipse::Component* aComponent, NetMessage& outMessage, bool aStartLater = false);
        static void CreatePrefabMessage(unsigned aGOID, const GUID& PrefabAssetID, std::vector<unsigned>, NetMessage& outMessage);

    public:
        // Direct send functions
        static void SendPrefabObject(GameObject* gameobject, Assets::Prefab& aPrefab);

    public:
        static void SetBeforeReplicatedList();
        static void SetAfterReplicatedList();

    private:
        static inline ReplicationVariableMap* BeforeReplicatedVariableList;

    public:
        static inline ReplicationVariableMap TemporaryReplicatedVariableList;

        static inline ReplicationVariableMap BeforePlayReplicatedVariables;
        static inline ReplicationVariableMap OnPlayReplicatedVariables;

        static inline ReplicationVariableMap* CurrentReplicatedVariabpePtr = &BeforePlayReplicatedVariables;

        static inline SteamP2PNetworkingClient* client = nullptr;
        static inline SteamP2PNetworkingServer* server = nullptr;

        static inline bool startServer = true;
        static inline bool startClient = false;

        static inline bool startedGame = false;

        static inline asio::io_context ioContext;

        static inline std::string IP = "";

        static inline bool ClickedHostButton = false;
    };
}

#endif