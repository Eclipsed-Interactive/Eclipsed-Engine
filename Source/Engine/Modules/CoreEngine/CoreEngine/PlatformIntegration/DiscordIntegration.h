#pragma once

#include <vector>
#include <string>
#include <functional>

namespace discord
{
	class Core;
	class Activity;
}

namespace Eclipse::PlatformIntegration
{
	class Discord
	{
	public:
		static void SetupWithID(const long long& id);
		static void Setup();
		static void Stop();
		static void Update();

		static void SetApplicationID(const long long& id);

		/// <summary>
		/// The description of the status message.
		/// </summary>
		/// <param name="details">The description.</param>
		static void SetDetails(const char* details);

		static void SetLargeText(const char* text);
		static void SetLargeImage(const char* imageID);

		static void SetSmallText(const char* text);
		static void SetSmallImage(const char* imageID);

		static void SetState(const char* state);

		static bool IsConnected();

	private:
		static discord::Core* core;
		static discord::Activity* activity;

		static bool isDirty;
		static bool isActive;
		static long long applicationID;
	};
}