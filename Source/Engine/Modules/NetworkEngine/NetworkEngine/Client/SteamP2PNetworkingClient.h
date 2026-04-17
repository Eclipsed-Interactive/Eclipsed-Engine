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

            SteamNetworkingMessage_t* receivedMessages[256];
            int messageCount = SteamNetworkingSockets()->ReceiveMessagesOnConnection(myConnection, receivedMessages, 256);

            if (messageCount < 0)
                return;

            for (int i = 0; i < messageCount; ++i)
            {
                SteamNetworkingMessage_t* msg = receivedMessages[i];

                NetMessage message;
                memcpy(&message, msg->GetData(), msg->m_cbSize);
        
                msg->Release();
        
                HandleRecieve(message);
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
