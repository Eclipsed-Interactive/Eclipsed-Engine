#include "PerformanceProfiler.h"

namespace Eclipse::Debug
{
	void PerformanceProfiler::BeginFrame()
	{
	}

	void PerformanceProfiler::EndFrame()
	{
	}

	void PerformanceProfiler::BeginScope(const ProfilerEntry& anEntry)
	{
	}

	void PerformanceProfiler::EndScope()
	{
	}

	void PerformanceProfiler::StartRecording()
	{
		isRecording = true;
	}
	
	void PerformanceProfiler::StopRecording()
	{
		isRecording = false;
	}
}