#include "PlayModeController.h"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "Core/Timer.h"

namespace Eclipse
{
	bool PlayModeController::StepRequested = false;
	PlayMode PlayModeController::CurrentMode = PlayMode::Stopped;
	PlayMode PlayModeController::RequestedMode = PlayMode::Stopped;

	PlayMode PlayModeController::GetPlayMode()
	{
		return CurrentMode;
	}

	void PlayModeController::SetPlayMode(PlayMode mode)
	{
		RequestedMode = mode;
	}

	void PlayModeController::Step()
	{
		if (CurrentMode == PlayMode::Paused)
			StepRequested = true;
	}

	bool PlayModeController::ShouldTick()
	{
		if (CurrentMode == PlayMode::Playing)
			return true;

		if (CurrentMode == PlayMode::Paused && StepRequested)
		{
			StepRequested = false;
			return true;
		}

		return false;
	}

	void PlayModeController::EnterPlay()
	{
		SetPlayMode(PlayMode::Playing);

		Timer::SetTimeScale(1.f);
	}

	void PlayModeController::ExitPlay()
	{
		SetPlayMode(PlayMode::Stopped);

		Timer::SetTimeScale(1.f);
		SceneManager::ReloadActive();
	}

	bool PlayModeController::IsPlaying()
	{
		return CurrentMode == PlayMode::Playing;
	}

	bool PlayModeController::IsPaused()
	{
		return CurrentMode == PlayMode::Paused;
	}
	
	bool PlayModeController::IsInPlaySession()
	{
		return CurrentMode != PlayMode::Stopped;
	}

	void PlayModeController::ApplyMode()
	{
		CurrentMode = RequestedMode;

		switch (CurrentMode)
		{
		case Eclipse::PlayMode::Playing:
			EnterPlay();
			break;
		case Eclipse::PlayMode::Paused:
			// Pause
			break;
		case Eclipse::PlayMode::Stopped:
			ExitPlay();
			break;
		}
	}

}