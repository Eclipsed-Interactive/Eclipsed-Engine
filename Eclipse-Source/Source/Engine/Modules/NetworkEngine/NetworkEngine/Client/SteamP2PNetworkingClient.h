#pragma once

#include "steamsdk/isteamnetworkingsockets.h"

class SteamP2PNetworkingClient
{
public:
    static SteamP2PNetworkingClient& Get()
    {
        static SteamP2PNetworkingClient Instance;
        return Instance;
    }
    
    void Start(uint64 aSteamID)
    {
        CSteamID steamID(aSteamID);
        
        SteamNetworkingIdentity identity;
        identity.SetSteamID(steamID);
        
        SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
    }
    
public:
    HSteamNetConnection myConnection;
};
