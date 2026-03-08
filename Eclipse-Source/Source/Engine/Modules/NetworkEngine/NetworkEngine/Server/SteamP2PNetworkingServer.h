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
    
public:
    HSteamListenSocket myListenSocket;
};
