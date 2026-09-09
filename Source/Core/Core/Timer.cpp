#include "pch.h"  

#include "Timer.h"
#include <chrono>

namespace Eclipse
{
    std::chrono::high_resolution_clock::time_point Timer::startTime;
    std::chrono::high_resolution_clock Timer::clock;
    std::chrono::duration<float> Timer::duration;

    float Timer::myDeltaTime;
    float Timer::myTotalTime;
    float Timer::myTimeScale;

    void Timer::Init()
    {
        startTime = clock.now();
        duration = std::chrono::duration<float>(0);
    }

    void Timer::Update()
    {
        duration = clock.now() - startTime;
        float newTotalTime = duration.count();
        float deltaTimeNoScaled = newTotalTime - myTotalTime;
        myDeltaTime = deltaTimeNoScaled * myTimeScale;
        myTotalTime = newTotalTime;
    }

    void Timer::SetTimeScale(float timeScale)
    {
        myTimeScale = timeScale;
    }

    float Timer::GetDeltaTime()
    {
        return myDeltaTime * myTimeScale;
    }

    float Timer::GetTimeScale()
    {
        return myTimeScale;
    }

    float Timer::GetTotalTime()
    {
        return myTotalTime;
    }
}