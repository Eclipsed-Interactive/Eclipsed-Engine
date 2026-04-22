#pragma once
#include "CoreEngine/Math/Color.h"
#include "CoreEngine/Math/Vector/Vector2.h"

#include "GraphicsEngine/OpenGL/DebugDrawers/DebugDrawer.h"

inline void DrawLine(Eclipse::Math::Vector2f aStart, Eclipse::Math::Vector2f aEnd, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawLine(aStart * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aEnd * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), false, aColor);
}
inline void DrawRay(Eclipse::Math::Vector2f aStartPos, Eclipse::Math::Vector2f aDirection, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawRay(aStartPos * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aDirection, aColor);
}
inline void DrawArrow(Eclipse::Math::Vector2f aStartPos, Eclipse::Math::Vector2f aDirection, float aLineLength = 1.f, float anArrowSpan = 0.1f, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawArrow(aStartPos * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aDirection, aLineLength, anArrowSpan, aColor);
}

inline void DrawSquare(Eclipse::Math::Vector2f aPosition, float aRotation, Eclipse::Math::Vector2f aHalfExtents, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawSquare(aPosition * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aRotation, aHalfExtents, aColor);
}
inline void DrawSquareMinMax(Eclipse::Math::Vector2f aMinPosition, Eclipse::Math::Vector2f aMaxPosition, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawSquareMinMax(aMinPosition * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aMaxPosition * 0.5f  + Eclipse::Math::Vector2f(1, 1), aColor);
}

inline void DrawCircle(Eclipse::Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution = 16, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawCircle(aPosition * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aRadius, aCircleResolution, aColor);
}
inline void DrawHalfCircle(Eclipse::Math::Vector2f aPosition, float aRadius, const Eclipse::Math::Vector2f& aDirection, unsigned aCircleResolution = 16, const Eclipse::Math::Color& aColor = Eclipse::Math::Color(0, 1, 0, 1))
{
    Eclipse::DebugDrawer::DrawHalfCircle(aPosition * 0.5f + Eclipse::Math::Vector2f(0.5f, 0.5f), aRadius, aDirection, aCircleResolution, aColor);
}