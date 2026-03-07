#include "SteamGeneral.h"

#include "steamSdk/steam_api.h"
#include "steamSdk/isteammatchmaking.h"

namespace Eclipse
{
    void SteamGeneral::Init()
    {
        SteamErrMsg error;
        SteamAPI_InitEx(&error);
        
        if (*error != 0)
        {
            assert("Something went wrong with initing steamSDK" && false);
        }
        
        SteamFriends()->SetRichPresence("steam_player_group", "192.168.1.202");
    }
}
