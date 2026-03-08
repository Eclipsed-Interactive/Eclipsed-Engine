#pragma once

#include "steamsdk/isteamnetworkingsockets.h"

enum EMessageType
{
    Reliable = k_nSteamNetworkingSend_Reliable,
    Garantied = k_nSteamNetworkingSend_Reliable,
    Ack = k_nSteamNetworkingSend_Reliable,
    
    Unreliable = k_nSteamNetworkingSend_Unreliable,
    NotGarantied = k_nSteamNetworkingSend_Unreliable,
    NotAck = k_nSteamNetworkingSend_Unreliable,

    NoNagle = k_nSteamNetworkingSend_NoNagle,
    NoDelay = k_nSteamNetworkingSend_NoDelay,
};

class SteamP2PNetworkingClient
{
public:
    static SteamP2PNetworkingClient& Get()
    {
        static SteamP2PNetworkingClient Instance;
        return Instance;
    }

    void Update()
    {
        
    }
    
    void Start(uint64 aSteamID)
    {
        CSteamID steamID(aSteamID);
        
        SteamNetworkingIdentity identity;
        identity.SetSteamID(steamID);
        
        myConnection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
    }

    void Send(const void* aData, uint32 aDataCount, EMessageType messageType)
    {
        SteamNetConnectionInfo_t status;
        if (SteamNetworkingSockets()->GetConnectionInfo(myConnection, &status)) {
            printf("Connection state: %d\n", status.m_eState);
            // Should be k_ESteamNetworkingConnectionState_Connected (which is 4)
        }
        
        int64 messageCount;
        EResult result = SteamNetworkingSockets()->SendMessageToConnection(myConnection, aData, aDataCount, messageType, &messageCount);

        SteamNetworkingSockets()->FlushMessagesOnConnection(myConnection);
    }
    
public:
    HSteamNetConnection myConnection;
};
