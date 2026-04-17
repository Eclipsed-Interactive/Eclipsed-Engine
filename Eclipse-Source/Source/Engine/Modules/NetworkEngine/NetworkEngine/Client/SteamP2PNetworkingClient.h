#pragma once

#include <functional>

#include "EclipsedEngine/Replication/ReplicationManager.h"
#include "steamsdk/isteamnetworkingsockets.h"
#include "steamsdk/isteamnetworkingutils.h"

#include "NetworkEngine/Shared/Message.h"

#include <iostream>

namespace Eclipse
{
    class SteamP2PNetworkingClient
    {
    public:
        SteamP2PNetworkingClient(std::function<void(const NetMessage& aNetMessage)> aHandleRecieve) : myConnection(0), HandleRecieve(aHandleRecieve)
        {
            
        }

        void CloseConnection(const char* aReason)
        {
            SteamNetworkingSockets()->CloseConnection(myConnection, 1, aReason, true);
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

        STEAM_CALLBACK(SteamP2PNetworkingClient, OnSteamConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
    };

    inline void SteamP2PNetworkingClient::OnSteamConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* aInfo)
    {
        switch (aInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_Connected:
            Replication::ReplicationManager::SteamNetorkingReady();
            break;

        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            printf("Connection failed with reason: %d - %s\n",
                   aInfo->m_info.m_eEndReason,
                   aInfo->m_info.m_szEndDebug);
            break;
        }

        myConnection = aInfo->m_hConn;
    }
}
