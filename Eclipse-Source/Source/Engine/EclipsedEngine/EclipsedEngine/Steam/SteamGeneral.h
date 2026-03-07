#pragma once

#include "steamSdk/steam_api.h"
#include "steamsdk/isteamfriends.h"

#include "steamSdk/steamclientpublic.h"

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
    };
}
