#pragma once

#include <functional>

#include "steamsdk/isteamnetworkingsockets.h"
#include "steamsdk/isteamnetworkingutils.h"

#include "NetworkEngine/Shared/Message.h"

namespace Eclipse
{
    class SteamP2PNetworkingClient
    {
    public:
        SteamP2PNetworkingClient(std::function<void(const NetMessage& aNetMessage)> aHandleRecieve) : myConnection(0), HandleRecieve(aHandleRecieve)
        {
            
        }
        
        void Update()
        {
            if (!myConnection)
                return;
        
            SteamNetworkingMessage_t* RecieveMessage = nullptr;
            int messageCount = SteamNetworkingSockets()->ReceiveMessagesOnConnection(myConnection, &RecieveMessage, 1);

            for (int i = 0; i < messageCount; ++i)
            {
                int messageSize = RecieveMessage[i].m_cbSize;
            
                NetMessage message;
                memcpy(&message, RecieveMessage[i].GetData(), messageSize);
                HandleRecieve(message);

                RecieveMessage[i].Release();
            }
        }
    
        void Start(CSteamID aSteamID)
        {        
            SteamNetworkingIdentity identity;
            identity.SetSteamID(aSteamID);
        
            myConnection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);

            SteamNetworkingUtils()->InitRelayNetworkAccess();
        }

        void Send(NetMessage& message)
        {
            EMessageType messageType;
            if (message.MetaData.IsGarantied)
                messageType = EMessageType::Garantied;
            else
                messageType = EMessageType::NotGarantied;
            
            int64 messageCount;
            EResult result = SteamNetworkingSockets()->SendMessageToConnection(myConnection, &message, message.MetaData.dataSize, messageType, &messageCount);

            SteamNetworkingSockets()->FlushMessagesOnConnection(myConnection);
        }
    
    public:
        HSteamNetConnection myConnection;

        std::function<void(const NetMessage& aNetMessage)> HandleRecieve;
    };
}
