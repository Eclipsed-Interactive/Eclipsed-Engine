#include "SteamGeneral.h"

#include "Replication/ReplicationManager.h"

namespace Eclipse
{
    
    void SteamGeneral::RelayNetworkInitilized(SteamRelayNetworkStatus_t* aCallback)
    {
        Replication::ReplicationManager::RelayNetworkReady();
    }
    
    void SteamGeneral::FriendClickedJoinedGame(GameRichPresenceJoinRequested_t* aCallback)
    {
        OthersteamID = aCallback->m_steamIDFriend;
        
        Replication::ReplicationManager::Start();
    }

    bool SteamGeneral::SetRichPresence( const char *pchKey, const char *pchValue)
    {
        return SteamFriends()->SetRichPresence(pchKey, pchValue);
    }

    void SteamGeneral::Update() 
    {
        if (Input::GetKeyDown(Keycode::U))
        {
            Replication::ReplicationManager::Start();
        }
        
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

        SetRichPresence("connect", "1");
    }

    void SteamGeneral::ShutDown()
    {
        SteamFriends()->ClearRichPresence();
        SteamAPI_Shutdown();
    }
}
