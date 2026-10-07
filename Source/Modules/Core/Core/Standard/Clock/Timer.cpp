#include "Timer.h"

#include "Clock.h"

namespace Eclipse
{
    TimePoint Timerr::myPrevious;
    Duration Timerr::myDelta;
    Duration Timerr::myTotal;

    float Timerr::myTimeScale = 0.f;

    void Timerr::Tick()
    {
        TimePoint now = Clock::Now();

        myDelta = now - myPrevious;
        myPrevious = now;
        myTotal += myDelta;
    }

    Duration Timerr::DeltaTime() 
    {
        return myDelta * myTimeScale;
    }

    Duration Timerr::TotalTime() 
    {
        return myTotal.ToSeconds();
    }

    float Timerr::GetDeltaTime() 
    {
        return myDelta.ToSeconds();
    }

    float Timerr::GetTotalTime() 
    {
        return myTotal.ToSeconds();
    }

    float Timerr::GetTimeScale() 
    {
        return myTimeScale;
    }

    void Timerr::SetTimeScale(float timeScale)
    {
        myTimeScale = timeScale;
    }
}