#pragma once

#include <chrono>

namespace Eclipse
{
	class Time
	{
	public:
		Time() = delete;
		~Time() = delete;

	public:
		static void Init();
		static void Update();

		static void SetTimeScale(float timeScale);

		static float GetDeltaTime();
		static float GetTotalTime();
		static float GetTimeScale();

	private:
		static std::chrono::high_resolution_clock::time_point startTime;
		static std::chrono::high_resolution_clock clock;
		static std::chrono::duration<float> duration;

		static float myDeltaTime;
		static float myTotalTime;
		static float myTimeScale;
	};
}