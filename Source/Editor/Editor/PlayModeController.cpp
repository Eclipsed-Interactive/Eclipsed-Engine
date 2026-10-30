#include "PlayModeController.h"

#include "EclipsedEngine/Scenes/SceneManager.h"
#include "Core/Timer.h"

#include <ECS/ComponentManager.h>

namespace Eclipse
{
	bool PlayModeController::StepRequested = false;
	PlayMode PlayModeController::CurrentMode = PlayMode::Stopped;
	PlayMode PlayModeController::RequestedMode = PlayMode::Stopped;
	std::string PlayModeController::SceneSnapshot = "";


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
		ComponentManager::AwakeStartComponents();
		SceneSnapshot = SceneManager::CaptureSnapshot();
		Timer::SetTimeScale(1.f);
	}

	void PlayModeController::ExitPlay()
	{
		Timer::SetTimeScale(1.f);
		SceneManager::RestoreSnapshot(SceneSnapshot);
		SceneSnapshot.clear();
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
		if (CurrentMode == RequestedMode)
			return;

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