#include "SteamGeneral.h"

#include "Replication/ReplicationManager.h"

namespace Eclipse
{
    
    void SteamGeneral::RelayNetworkInitilized(SteamRelayNetworkStatus_t* aCallback)
    {
        Replication::ReplicationManager::RelayNetworkReady();
    }
    
    void SteamGeneral::Update()
    {
        SteamAPI_RunCallbacks();
    }
    
    void SteamGeneral::Init()
    {
        SteamErrMsg error;
        if (SteamAPI_InitEx(&error) != k_ESteamAPIInitResult_OK)
        {
            printf("SteamAPI Init failed: %s\n", error);
            assert(false && "Steam initialization failed");
            return;
        }

        SteamFriends()->SetRichPresence("connect", "connect");
    }
}
