#pragma once

#include <string>

namespace Eclipse
{
	enum class PlayMode
	{
		Playing,
		Paused,
		Stopped
	};

	class PlayModeController
	{
	public:
		static PlayMode GetPlayMode();
		static void SetPlayMode(PlayMode playMode);

		static void Step();

		static void ApplyMode();

	private:
		static void EnterPlay();
		static void ExitPlay();

	public:
		static bool ShouldTick();

		static bool IsPlaying();
		static bool IsPaused();
		static bool IsInPlaySession();

	private:
		static bool StepRequested;
		static PlayMode CurrentMode;
		static PlayMode RequestedMode;

		static std::string SceneSnapshot;
	};
}