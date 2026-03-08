#pragma once


#include "SteamP2PNetworkingServer.h"
#include "steamsdk/isteamnetworkingsockets.h"
#include "steamsdk/isteamgameserver.h"

class SteamP2PNetworkingServer
{
public:
    static SteamP2PNetworkingServer& Get()
    {
        static SteamP2PNetworkingServer Instance;
        return Instance;
    }

    void Start()
    {
        if (SteamGameServer())
        {
            SteamGameServer()->SetProduct("Project Nova");
            SteamGameServer()->SetGameDescription("Project Nova Desc");

            //SteamGameServer()->SetSpectatorPort( ... );
            //SteamGameServer()->SetSpectatorServerName( ... );

            SteamGameServer()->LogOnAnonymous();

            SteamNetworkingUtils()->InitRelayNetworkAccess();
        }
        else
        {
            OutputDebugString("SteamGameServer() interface is invalid\n");
        }

        myListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
    }

    void Update()
    {
        if (!myConnection)
            return;
        
        SteamNetworkingMessage_t* message = nullptr;
        int messageCount = SteamNetworkingSockets()->ReceiveMessagesOnConnection(myConnection, &message, 32);

        for (int i = 0; i < messageCount; ++i)
        {
            int messageSize = message->m_cbSize;
            
            char* mess = static_cast<char*>(malloc(messageSize));
            memcpy(mess, message->GetData(), messageSize);
            memset(mess + messageSize, '\0', 1);
            message[i].Release();
        }
    }

public:
    HSteamListenSocket myListenSocket;
    HSteamNetConnection myConnection;

    STEAM_CALLBACK(SteamP2PNetworkingServer, OnSteamConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
};

inline void SteamP2PNetworkingServer::OnSteamConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* aInfo)
{
    switch (aInfo->m_info.m_eState) {
    case k_ESteamNetworkingConnectionState_Connecting:
        // IMPORTANT: Accept immediately when in Connecting state
        SteamNetworkingSockets()->AcceptConnection(aInfo->m_hConn);
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
