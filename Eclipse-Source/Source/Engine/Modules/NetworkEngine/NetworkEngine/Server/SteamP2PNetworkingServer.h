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
        int messageCount = SteamNetworkingSockets()->ReceiveMessagesOnConnection(myConnection, &message, 1);

        if (messageCount)
        {
            int uhadsud = 867;
        }
    }

public:
    HSteamListenSocket myListenSocket;
    HSteamNetConnection myConnection;

    STEAM_CALLBACK(SteamP2PNetworkingServer, OnSteamConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);
};

inline void SteamP2PNetworkingServer::OnSteamConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* aInfo)
{
    myConnection = aInfo->m_hConn;
}
