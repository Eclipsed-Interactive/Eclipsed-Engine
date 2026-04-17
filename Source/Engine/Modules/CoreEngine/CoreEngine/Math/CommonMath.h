#pragma once

namespace Eclipse::Math
{
    constexpr float pi = 3.141592653589f;
    constexpr float piHalf = 1.57079632679f; // Half pi, 90 degree rotation
    constexpr float piQuat = 0.78539816339f; // One fourth of pi, 45 degree rotation
    constexpr float piEight = 0.39269908169f; // One eight of pi, 22.5 degree rotation
    constexpr float pi2 = 6.28318530718f; // two pi, 360 degree rotation

    constexpr float rad2Deg = 180.0f / pi;
    constexpr float deg2Rad = (1.0f / 180.0f * pi);


    template<typename T>
    inline T Max(const T& lhs, const T& rhs) {
        if (lhs > rhs) return lhs;
        else return rhs;
    }

    template<typename T>
    inline T Min(const T& lhs, const T& rhs) {
        if (lhs < rhs) return lhs;
        else return rhs;
    }
}
