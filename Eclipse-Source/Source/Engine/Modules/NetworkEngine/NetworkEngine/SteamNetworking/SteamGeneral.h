#pragma once

#include "steamSdk/steamclientpublic.h"

namespace Eclipse
{
    class SteamGeneral
    {
    public:
        SteamGeneral() = default;

        void Init();

        static SteamGeneral Get()
        {
            static SteamGeneral instance;
            return instance;
        }
        CSteamID mySteamID;

    private:
    };
}
