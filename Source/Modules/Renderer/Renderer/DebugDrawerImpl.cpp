#include "IDebugDrawer.h"

#include "IDebugDrawer.h"

void Eclipse::DebugDrawer::DrawLine(Math::Vector2f aStart, Math::Vector2f aEnd, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawLine(aStart, aEnd, false, aColor);
}

void Eclipse::DebugDrawer::DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawRay(aStartPos, aDirection, aColor);
}

void Eclipse::DebugDrawer::DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength, float anArrowSpan, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawArrow(aStartPos, aDirection, aLineLength, anArrowSpan, aColor);
}

void Eclipse::DebugDrawer::DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawSquare(aPosition, aRotation, aHalfExtents, aColor);
}

void Eclipse::DebugDrawer::DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawSquareMinMax(aMinPosition, aMaxPosition, aColor);
}

void Eclipse::DebugDrawer::DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawCircle(aPosition, aRadius, aCircleResolution, aColor);
}

void Eclipse::DebugDrawer::DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution, const Math::Color& aColor)
{
	Graphics::IDebugDrawer::DrawHalfCircle(aPosition, aRadius, aDirection, aCircleResolution, aColor);
}