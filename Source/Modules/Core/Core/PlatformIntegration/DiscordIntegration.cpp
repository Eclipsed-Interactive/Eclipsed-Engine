//#include "pch.h"
//
//#include "DiscordIntegration.h"
//
//#include "DiscordSDK/discord.h"
//
//#include <assert.h>
//#include <string>
//
//namespace Eclipse::PlatformIntegration
//{
//#ifdef ENABLE_DISCORD_SDK
//	discord::Activity* Discord::activity = {};
//	discord::Core* Discord::core = {};
//	bool Discord::isDirty = false;
//
//	bool Discord::isActive = false;
//	long long Discord::applicationID = 0;
//
//	void Discord::SetupWithID(const long long& id)
//	{
//		SetApplicationID(id);
//		Setup();
//	}
//
//	void Discord::Setup()
//	{
//		activity = new discord::Activity();
//		activity->SetInstance(true);
//
//		discord::Result result = discord::Core::Create(applicationID, DiscordCreateFlags_NoRequireDiscord, &core);
//		if (result == discord::Result::Ok)
//		{
//			isActive = true;
//		}
//	}
//
//	void Discord::Stop()
//	{
//		delete activity;
//		delete core;
//	}
//
//	void Discord::Update()
//	{
//		if (!IsConnected())
//		{
//			Setup();
//			return;
//		}
//
//		if (isDirty)
//		{
//			core->ActivityManager().UpdateActivity(*activity, [](discord::Result res) {});
//			isDirty = false;
//		}
//
//		core->RunCallbacks();
//	}
//
//	void Discord::SetApplicationID(const long long& id)
//	{
//		memcpy(&applicationID, &id, sizeof(long long));
//	}
//
//	void Discord::SetDetails(const char* details)
//	{
//		activity->SetDetails(details);
//		isDirty = true;
//	}
//
//	void Discord::SetLargeText(const char* text)
//	{
//		activity->GetAssets().SetLargeText(text);
//		isDirty = true;
//	}
//
//	void Discord::SetLargeImage(const char* imageID)
//	{
//		activity->GetAssets().SetLargeImage(imageID);
//		isDirty = true;
//	}
//
//	void Discord::SetSmallText(const char* text)
//	{
//		activity->GetAssets().SetSmallText(text);
//		isDirty = true;
//	}
//
//	void Discord::SetSmallImage(const char* imageID)
//	{
//		activity->GetAssets().SetSmallImage(imageID);
//		isDirty = true;
//	}
//
//	void Discord::SetState(const char* state)
//	{
//		activity->SetState(state);
//		isDirty = true;
//	}
//
//	bool Discord::IsConnected()
//	{
//		return isActive;
//	}
//#else 
//	void Discord::SetupWithID(const long long& id) {}
//
//	void Discord::Setup() {}
//
//	void Discord::Stop() {}
//
//	void Discord::Update() {}
//
//	void Discord::SetApplicationID(const long long& id) {}
//
//	void Discord::SetDetails(const char* details) {}
//
//	void Discord::SetLargeText(const char* text) {}
//	void Discord::SetLargeImage(const char* imageID) {}
//	void Discord::SetSmallText(const char* text) {}
//	void Discord::SetSmallImage(const char* imageID) {}
//	void Discord::SetState(const char* state) {}
//	bool Discord::IsConnected() { return false; }
//#endif
//}