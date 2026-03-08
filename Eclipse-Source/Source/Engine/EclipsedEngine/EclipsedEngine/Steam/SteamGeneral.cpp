#include "SteamGeneral.h"

#include "steamsdk/isteamnetworkingutils.h"

namespace Eclipse
{
    void SteamGeneral::FriendClickedJoinedGame(GameRichPresenceJoinRequested_t* aCallback)
    {
        // const char* friendPresence = SteamFriends()->GetFriendRichPresence(aCallback->m_steamIDFriend, "connect");
        //
        // Replication::ReplicationManager::IP = friendPresence;
        // Replication::ReplicationManager::startClient = true;
        // Replication::ReplicationManager::startServer = false;
        //
        // Replication::ReplicationManager::Start();
    }
    
    void SteamGeneral::Update()
    {
        SteamAPI_RunCallbacks();
    }
    
    void SteamGeneral::Init()
    {
        SteamErrMsg error;
        SteamAPI_InitEx(&error);
        
        if (*error != 0)
            assert((std::string("Something went wrong with initing steamSDK Error: ") + std::string(error)).c_str() && false);

        //SteamFriends()->SetRichPresence("connect", General::GetPublicIPAdress().c_str());

        SteamNetworkingUtils()->InitRelayNetworkAccess();
    }
}
