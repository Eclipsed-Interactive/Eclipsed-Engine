#ifdef ECLIPSED_NETWORKING

#include "ReplicatedVariable.h"
#include "Networking/Client/SteamP2PNetworkingClient.h"
#include "Networking/Server/SteamP2PNetworkingServer.h"
#include <Core/MainSingleton.h>


namespace Eclipse::Replication
{
    void BaseReplicatedVariable::ReplicateThis(unsigned aID, bool aIsGarantied, bool aAlwaysSend)
    {
        if (!OnComponent->IsReplicated || !OnComponent->IsOwner() && !aAlwaysSend)
            return;

        unsigned componentID = OnComponent->myInstanceComponentID;

        char* data = reinterpret_cast<char*>(malloc(sizeof(componentID) + sizeof(IterationID) + sizeof(DataCount) + DataCount));

        size_t offset = 0;

        memcpy(data + offset, &componentID, sizeof(componentID));
        offset += sizeof(componentID);

        memcpy(data + offset, &IterationID, sizeof(IterationID));
        offset += sizeof(IterationID);

        memcpy(data + offset, &DataCount, sizeof(DataCount));
        offset += sizeof(DataCount);

        //if (IsAsset)
        //{
        //    Assets::AssetData** handle = reinterpret_cast<Assets::AssetData**>(Data);
        //    memcpy(data + offset, *handle, DataCount);
        //    offset += DataCount;
        //}
        //else
        //{
            memcpy(data + offset, Data, DataCount);
            offset += DataCount;
        //}

        if (Eclipse::MainSingleton::Exists<SteamP2PNetworkingClient>())
        {
            NetMessage message = NetMessage::BuildGameObjectMessage(OnComponent->gameObject->GetID(), MessageType::Msg_Variable, data, offset, aIsGarantied);
            SteamP2PNetworkingClient& client = Eclipse::MainSingleton::GetInstance<SteamP2PNetworkingClient>();
            client.Send(message);   
        }
        else if (Eclipse::MainSingleton::Exists<SteamP2PNetworkingServer>())
        {
            NetMessage message = NetMessage::BuildGameObjectMessage(OnComponent->gameObject->GetID(), MessageType::Msg_Variable, data, offset, aIsGarantied);
            SteamP2PNetworkingServer& server = Eclipse::MainSingleton::GetInstance<SteamP2PNetworkingServer>();
            server.Send(message);   
        }

        free(data);
    }
}

#endif