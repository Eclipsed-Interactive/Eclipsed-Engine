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
    
    void Start(uint32 aSteamID, EUniverse aUniverse)
    {
        CSteamID steamID(aSteamID, aUniverse, EAccountType::k_EAccountTypeIndividual);
        
        SteamNetworkingIdentity identity;
        identity.SetSteamID(steamID);
        
        SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
    }
    
public:
    HSteamNetConnection myConnection;
};
