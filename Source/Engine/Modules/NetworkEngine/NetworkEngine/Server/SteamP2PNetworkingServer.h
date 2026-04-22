#pragma once

#include <functional>

#include "steamsdk/isteamnetworkingsockets.h"
#include "steamsdk/isteamgameserver.h"
#include "steamsdk/isteamnetworkingutils.h"

#include "NetworkEngine/Shared/Message.h"

#include <iostream>

namespace Eclipse
{
    class SteamP2PNetworkingServer
    {
    public:
        SteamP2PNetworkingServer(const std::function<void(const NetMessage& aNetMessage)>& aHandleRecieve) : myConnection(0), myListenSocket(0), HandleRecieve(aHandleRecieve)
        {
        }

        void CloseConnection(const char* aReason)
        {
            SteamNetworkingSockets()->CloseConnection(myConnection, 1, aReason, true);
            SteamNetworkingSockets()->CloseListenSocket(myListenSocket);
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

        void Start()
        {
            SteamNetworkingUtils()->InitRelayNetworkAccess();

            myListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
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

    public:
        std::function<void(const NetMessage& aNetMessage)> HandleRecieve;

        HSteamListenSocket myListenSocket;
        HSteamNetConnection myConnection;

        STEAM_CALLBACK(SteamP2PNetworkingServer, OnSteamConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
    };

    inline void SteamP2PNetworkingServer::OnSteamConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* aInfo)
    {
        switch (aInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_Connecting:
            SteamNetworkingSockets()->AcceptConnection(aInfo->m_hConn);
            break;

        case k_ESteamNetworkingConnectionState_Connected:
            Replication::ReplicationManager::SteamNetorkingReady();
            break;

        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            // Log the reason code for debugging
            printf("Connection failed with reason: %d - %s\n",
                   aInfo->m_info.m_eEndReason,
                   aInfo->m_info.m_szEndDebug);
            break;
        }

        myConnection = aInfo->m_hConn;
    }
}
