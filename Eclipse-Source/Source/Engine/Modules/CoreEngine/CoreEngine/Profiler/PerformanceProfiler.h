#pragma once

#include <iostream>
#include <chrono>

namespace Eclipse::Debug
{
	// BEGIN_FRAME
	//	- BEGIN_SCOPE
	//	- END_SCOPE
	// END_FRAME

	// Keep x amount of frames.
	// Record button which captures every frame till stop recording.

	struct ProfilerEntry
	{
		std::string name;
	};

	class PerformanceProfiler final
	{
	public:
		static void BeginFrame();
		static void EndFrame();

		static void BeginScope(const ProfilerEntry& anEntry);
		static void EndScope();

	public:
		static void StartRecording();
		static void StopRecording();

	private:
		static inline bool isRecording = false;
	};
}