#include "pch.h"  

#include "Timer.h"
#include <chrono>

namespace Eclipse
{
    std::chrono::high_resolution_clock::time_point Time::startTime;
    std::chrono::high_resolution_clock Time::clock;
    std::chrono::duration<float> Time::duration;

    float Time::myDeltaTime = 0;
    float Time::myTotalTime = 0;
    float Time::myTimeScale = 1;

    void Time::Init()
    {
        startTime = clock.now();
    }

    void Time::Update()
    {
        duration = clock.now() - startTime;
        float newTotalTime = duration.count();
        float deltaTimeNoScaled = newTotalTime - myTotalTime;
        myDeltaTime = deltaTimeNoScaled * myTimeScale;
        myTotalTime = newTotalTime;
    }

    void Time::SetTimeScale(float timeScale)
    {
        myTimeScale = timeScale;
    }

    float Time::GetDeltaTime()
    {
        return myDeltaTime * myTimeScale;
    }

    float Time::GetTimeScale()
    {
        return myTimeScale;
    }

    float Time::GetTotalTime()
    {
        return myTotalTime;
    }
}