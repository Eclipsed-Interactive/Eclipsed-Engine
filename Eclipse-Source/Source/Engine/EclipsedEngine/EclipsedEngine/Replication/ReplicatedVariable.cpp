#include "ReplicatedVariable.h"

#include "AssetEngine/Data/AssetData.h"


namespace Eclipse::Replication
{
    void BaseReplicatedVariable::ReplicateThis(unsigned aID, bool aIsGarantied, bool aAlwaysSend)
    {
        if (!ConnectedComponent->IsReplicated || !ConnectedComponent->IsOwner() && !aAlwaysSend)
            return;

        unsigned componentID = ConnectedComponent->myInstanceComponentID;

        char* data = reinterpret_cast<char*>(malloc(sizeof(componentID) + sizeof(aID) + sizeof(dataAmount) + dataAmount));

        size_t offset = 0;
        memcpy(data + offset, &componentID, sizeof(componentID));
        offset += sizeof(componentID);

        memcpy(data + offset, &aID, sizeof(aID));
        offset += sizeof(aID);

        memcpy(data + offset, &dataAmount, sizeof(dataAmount));
        offset += sizeof(dataAmount);

        if (IsAsset)
        {
            AssetData** handle = reinterpret_cast<AssetData**>(myReflectVariable->GetData());
            memcpy(data + offset, *handle, dataAmount);
            offset += dataAmount;
        }
        else
        {
            memcpy(data + offset, myReflectVariable->GetData(), dataAmount);
            offset += dataAmount;
        }

        NetMessage message = NetMessage::BuildGameObjectMessage(ConnectedComponent->gameObject->GetID(), MessageType::Msg_Variable, data, offset, aIsGarantied);

        Client& client = Eclipse::MainSingleton::GetInstance<Client>();
        client.Send(message);

        free(data);
    }
}