#ifdef ECLIPSED_EDITOR
#pragma once

#include "CoreEngine/Files/FileWatcher.h"
#include "EclipsedEngine/EclipsedRuntime.h"

namespace Eclipse::Editor
{
	class EditorRuntime
	{
	public:
		bool IsPlaying();

	public:
		void Init(const std::string& path);

	private:
		void SetGameChanged(const FileWatcherEvent&);
		void LoadDLL();

	public:
		void UpdateGame();

		void UpdateEngine();
		void RenderEngine();
		bool BeginFrameEngine();
		void EndFrameEngine();

	public:
		void Shutdown();
		void EnterPlayMode();
		void ExitPlayMode();
		void PauseGame();
		void UnpauseGame();

	private:
		void DrawPlayGameButtons();

	private:
		bool isPlaying = false;
		bool isPaused = false;

		std::atomic<bool> gameChanged{ false };

		EclipsedRuntime eclipseRuntime = {};
	};
}
#endif