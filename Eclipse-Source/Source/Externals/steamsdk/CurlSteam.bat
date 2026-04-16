@echo off
curl -s https://eclipsed.se/sdk/isteamapps.h -o isteamapps.h
curl -s https://eclipsed.se/sdk/isteamappticket.h -o isteamappticket.h
curl -s https://eclipsed.se/sdk/isteamclient.h -o isteamclient.h
curl -s https://eclipsed.se/sdk/isteamcontroller.h -o isteamcontroller.h
curl -s https://eclipsed.se/sdk/isteamdualsense.h -o isteamdualsense.h
curl -s https://eclipsed.se/sdk/isteamfriends.h -o isteamfriends.h
curl -s https://eclipsed.se/sdk/isteamgamecoordinator.h -o isteamgamecoordinator.h
curl -s https://eclipsed.se/sdk/isteamgameserver.h -o isteamgameserver.h
curl -s https://eclipsed.se/sdk/isteamgameserverstats.h -o isteamgameserverstats.h
curl -s https://eclipsed.se/sdk/isteamhtmlsurface.h -o isteamhtmlsurface.h
curl -s https://eclipsed.se/sdk/isteamhttp.h -o isteamhttp.h
curl -s https://eclipsed.se/sdk/isteaminput.h -o isteaminput.h
curl -s https://eclipsed.se/sdk/isteaminventory.h -o isteaminventory.h
curl -s https://eclipsed.se/sdk/isteammatchmaking.h -o isteammatchmaking.h
curl -s https://eclipsed.se/sdk/isteammusic.h -o isteammusic.h
curl -s https://eclipsed.se/sdk/isteamnetworking.h -o isteamnetworking.h
curl -s https://eclipsed.se/sdk/isteamnetworkingmessages.h -o isteamnetworkingmessages.h
curl -s https://eclipsed.se/sdk/isteamnetworkingsockets.h -o isteamnetworkingsockets.h
curl -s https://eclipsed.se/sdk/isteamnetworkingutils.h -o isteamnetworkingutils.h
curl -s https://eclipsed.se/sdk/isteamparentalsettings.h -o isteamparentalsettings.h
curl -s https://eclipsed.se/sdk/isteamps3overlayrenderer.h -o isteamps3overlayrenderer.h
curl -s https://eclipsed.se/sdk/isteamremoteplay.h -o isteamremoteplay.h
curl -s https://eclipsed.se/sdk/isteamremotestorage.h -o isteamremotestorage.h
curl -s https://eclipsed.se/sdk/isteamscreenshots.h -o isteamscreenshots.h
curl -s https://eclipsed.se/sdk/isteamtimeline.h -o isteamtimeline.h
curl -s https://eclipsed.se/sdk/isteamugc.h -o isteamugc.h
curl -s https://eclipsed.se/sdk/isteamuser.h -o isteamuser.h
curl -s https://eclipsed.se/sdk/isteamuserstats.h -o isteamuserstats.h
curl -s https://eclipsed.se/sdk/isteamutils.h -o isteamutils.h
curl -s https://eclipsed.se/sdk/isteamvideo.h -o isteamvideo.h
curl -s https://eclipsed.se/sdk/matchmakingtypes.h -o matchmakingtypes.h
curl -s https://eclipsed.se/sdk/steam_api.h -o steam_api.h
curl -s https://eclipsed.se/sdk/steam_api.json -o steam_api.json
curl -s https://eclipsed.se/sdk/steam_api_common.h -o steam_api_common.h
curl -s https://eclipsed.se/sdk/steam_api_flat.h -o steam_api_flat.h
curl -s https://eclipsed.se/sdk/steam_api_internal.h -o steam_api_internal.h
curl -s https://eclipsed.se/sdk/steam_gameserver.h -o steam_gameserver.h
curl -s https://eclipsed.se/sdk/steamclientpublic.h -o steamclientpublic.h
curl -s https://eclipsed.se/sdk/steamencryptedappticket.h -o steamencryptedappticket.h
curl -s https://eclipsed.se/sdk/steamhttpenums.h -o steamhttpenums.h
curl -s https://eclipsed.se/sdk/steamnetworkingfakeip.h -o steamnetworkingfakeip.h
curl -s https://eclipsed.se/sdk/steamnetworkingtypes.h -o steamnetworkingtypes.h
curl -s https://eclipsed.se/sdk/steamps3params.h -o steamps3params.h
curl -s https://eclipsed.se/sdk/steamtypes.h -o steamtypes.h
curl -s https://eclipsed.se/sdk/steamuniverse.h -o steamuniverse.h

if not exist "lib" (
    mkdir lib
)
cd lib
if not exist "win64" (
    mkdir win64
)
cd win64

curl -s https://eclipsed.se/sdk/lib/win64/steam_api64.lib -o steam_api64.lib

echo SteamSDK acquired

pause