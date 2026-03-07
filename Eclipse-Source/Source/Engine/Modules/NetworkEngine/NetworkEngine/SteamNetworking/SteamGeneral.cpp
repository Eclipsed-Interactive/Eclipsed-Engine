#include "SteamGeneral.h"


#include "steamSdk/isteammatchmaking.h"

namespace Eclipse
{
    void SteamGeneral::FriendClickedJoinedGame(GameRichPresenceJoinRequested_t* aCallback)
    {
        const char* friendPresence = SteamFriends()->GetFriendRichPresence(aCallback->m_steamIDFriend, "connectIP");
    }
    
    void SteamGeneral::Init()
    {
        
        
        SteamErrMsg error;
        SteamAPI_InitEx(&error);
        
        if (*error != 0)
        {
            assert("Something went wrong with initing steamSDK" && false);
        }

        SteamFriends()->SetRichPresence("connectIP", "192.168.1.202");

        int asdf = 84354;
    }
}
