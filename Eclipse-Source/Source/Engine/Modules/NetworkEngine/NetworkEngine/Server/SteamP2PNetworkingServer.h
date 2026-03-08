#pragma once


#include "steamsdk/isteamnetworkingsockets.h"

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
        myListenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
    }

    void Update()
    {
        if (!myConnection)
            return;

        SteamNetworkingMessage_t* messages = new SteamNetworkingMessage_t[16];
        int messageCount = SteamNetworkingSockets()->ReceiveMessagesOnConnection(myConnection, &messages, 16);

        if (messageCount)
        {
            
        }

        for (int i = 0; i < 16; ++i)
        {
            std::cout << messages[i].GetData() << std::endl;
            messages[i].Release();
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
