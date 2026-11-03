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
		SceneSnapshot = SceneManager::CaptureSnapshot();
		Timer::SetTimeScale(1.f);
	}

	void PlayModeController::ExitPlay()
	{
		Timer::SetTimeScale(1.f);
		SceneManager::RestoreSnapshot(SceneSnapshot);
		SceneSnapshot.clear();
	}

	void PlayModeController::Pause()
	{
		ComponentManager::OnPauseComponents();
	}

	void PlayModeController::Unpause()
	{
		ComponentManager::OnUnpauseComponents();
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

		const PlayMode previousMode = CurrentMode;
		CurrentMode = RequestedMode;
		StepRequested = false;

		switch (previousMode)
		{
		case PlayMode::Stopped:
			// Stopped -> Playing / Paused: start a new play session.
			// Nothing has ticked yet, so there is nothing to pause.
			EnterPlay();
			break;

		case PlayMode::Playing:
			if (CurrentMode == PlayMode::Paused)
				Pause();
			else if (CurrentMode == PlayMode::Stopped)
				ExitPlay();
			break;

		case PlayMode::Paused:
			if (CurrentMode == PlayMode::Playing)
			{
				Unpause();
			}
			else if (CurrentMode == PlayMode::Stopped)
			{
				// Let components release paused resources before the scene is restored.
				Unpause();
				ExitPlay();
			}
			break;
		}
	}

}