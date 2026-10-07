#pragma once

#include "Core.Core.hpp"

#include "Duration.h"
#include "TimePoint.h"

namespace Eclipse
{
	class CORE_API Timerr final
	{
    public:
        static void Tick();

        static Duration DeltaTime();
        static Duration TotalTime();

    public:
        static float GetDeltaTime();
        static float GetTotalTime();
        static float GetTimeScale();

        static void SetTimeScale(float timeScale);

    private:
        static TimePoint myPrevious;
        static Duration myDelta;
        static Duration myTotal;

        static float myTimeScale ;
	};
}