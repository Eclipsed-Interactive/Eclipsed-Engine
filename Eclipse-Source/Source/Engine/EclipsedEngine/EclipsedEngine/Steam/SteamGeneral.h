#pragma once

#include "steamsdk/steam_api.h"
#include "steamsdk/isteamfriends.h"

#include "steamsdk/steamclientpublic.h"

namespace Eclipse
{
    class SteamGeneral
    {
    public:
        SteamGeneral() = default;

        void Update();
        void Init();

        static SteamGeneral& Get()
        {
            static SteamGeneral instance;
            return instance;
        }
        CSteamID mySteamID;

    private:
        STEAM_CALLBACK(SteamGeneral, FriendClickedJoinedGame, GameRichPresenceJoinRequested_t);
        STEAM_CALLBACK(SteamGeneral, RelayNetworkInitilized, SteamRelayNetworkStatus_t);
    };
}
