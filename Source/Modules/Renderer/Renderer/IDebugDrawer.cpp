#include "IDebugDrawer.h"

#include "Core/Math/CommonMath.h"
#include "Core/MainSingleton.h"

#include "Renderer/RendererManager.h"
#include "Renderer/IRenderer.h"
#include "Renderer/IGraphicsBuffer.h"

#include "Core/GraphicsBuffers/CameraBuffer.h"

void Eclipse::Graphics::IDebugDrawer::Begin()
{
	myLineCollection.clear();
}

void Eclipse::Graphics::IDebugDrawer::DrawLine(Math::Vector2f aStart, Math::Vector2f aEnd, bool aUsePrev, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	Line line;

	line.Pos1.posX = aStart.x;
	line.Pos1.posY = aStart.y;
	line.Pos1.color = aColor;

	line.Pos2.posX = aEnd.x;
	line.Pos2.posY = aEnd.y;
	line.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(line);
}

void Eclipse::Graphics::IDebugDrawer::DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	Line line;

	line.Pos1.posX = aStartPos.x;
	line.Pos1.posY = aStartPos.y;
	line.Pos1.color = aColor;

	line.Pos2.posX = aStartPos.x + aDirection.x;
	line.Pos2.posY = aStartPos.y + aDirection.y;
	line.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(line);
}

void Eclipse::Graphics::IDebugDrawer::DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength, float anArrowSpan, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	Math::Vector2f endPosition = aStartPos + aDirection * aLineLength;


	aDirection.Normalize();

	Line Centerline;
	Centerline.Pos1.posX = aStartPos.x;
	Centerline.Pos1.posY = aStartPos.y;
	Centerline.Pos1.color = aColor;

	Centerline.Pos2.posX = endPosition.x;
	Centerline.Pos2.posY = endPosition.y;
	Centerline.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(Centerline);


	Math::Vector2f arrowCornersStart = endPosition - aDirection * anArrowSpan;
	Math::Vector2f rightVector = Math::Vector2f(aDirection.y, -aDirection.x);
	Math::Vector2f rightArrowCorner = arrowCornersStart + rightVector * anArrowSpan;
	Math::Vector2f leftArrowCorner = arrowCornersStart - rightVector * anArrowSpan;

	Line Leftline;
	Leftline.Pos1.posX = endPosition.x;
	Leftline.Pos1.posY = endPosition.y;
	Leftline.Pos1.color = aColor;

	Leftline.Pos2.posX = leftArrowCorner.x;
	Leftline.Pos2.posY = leftArrowCorner.y;
	Leftline.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(Leftline);

	Line RightLine;
	RightLine.Pos1.posX = endPosition.x;
	RightLine.Pos1.posY = endPosition.y;
	RightLine.Pos1.color = aColor;

	RightLine.Pos2.posX = rightArrowCorner.x;
	RightLine.Pos2.posY = rightArrowCorner.y;
	RightLine.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(RightLine);
}

void RotateVtx(const Eclipse::Math::Vector2f& aPosition, Eclipse::Math::Vector2f& point, float aRotation)
{
	float cosTheta = cos(aRotation);
	float sinTheta = sin(aRotation);

	float dx = point.x - aPosition.x;
	float dy = point.y - aPosition.y;

	float rotatedX = dx * cosTheta - dy * sinTheta;
	float rotatedY = dx * sinTheta + dy * cosTheta;

	point.x = rotatedX + aPosition.x;
	point.y = rotatedY + aPosition.y;
}


void Eclipse::Graphics::IDebugDrawer::DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	Math::Vector2f BottomLeft = aPosition - aHalfExtents;
	Math::Vector2f TopLeft = aPosition + Math::Vector2f{ -aHalfExtents.x, aHalfExtents.y };

	Math::Vector2f TopRight = aPosition + aHalfExtents;
	Math::Vector2f BottomRight = aPosition + Math::Vector2f{ aHalfExtents.x, -aHalfExtents.y };

	RotateVtx(aPosition, BottomLeft, aRotation);
	RotateVtx(aPosition, TopLeft, aRotation);
	RotateVtx(aPosition, TopRight, aRotation);
	RotateVtx(aPosition, BottomRight, aRotation);

	{
		Line Line;
		Line.Pos1.posX = BottomLeft.x;
		Line.Pos1.posY = BottomLeft.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = TopLeft.x;
		Line.Pos2.posY = TopLeft.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = TopLeft.x;
		Line.Pos1.posY = TopLeft.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = TopRight.x;
		Line.Pos2.posY = TopRight.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = TopRight.x;
		Line.Pos1.posY = TopRight.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = BottomRight.x;
		Line.Pos2.posY = BottomRight.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = BottomRight.x;
		Line.Pos1.posY = BottomRight.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = BottomLeft.x;
		Line.Pos2.posY = BottomLeft.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
}

void Eclipse::Graphics::IDebugDrawer::DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	Math::Vector2f BottomLeft = aMinPosition;
	Math::Vector2f TopLeft = Math::Vector2f{ aMinPosition.x, aMaxPosition.y };

	Math::Vector2f TopRight = aMaxPosition;
	Math::Vector2f BottomRight = Math::Vector2f{ aMaxPosition.x, aMinPosition.y };

	{
		Line Line;
		Line.Pos1.posX = BottomLeft.x;
		Line.Pos1.posY = BottomLeft.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = TopLeft.x;
		Line.Pos2.posY = TopLeft.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = TopLeft.x;
		Line.Pos1.posY = TopLeft.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = TopRight.x;
		Line.Pos2.posY = TopRight.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = TopRight.x;
		Line.Pos1.posY = TopRight.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = BottomRight.x;
		Line.Pos2.posY = BottomRight.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
	{
		Line Line;
		Line.Pos1.posX = BottomRight.x;
		Line.Pos1.posY = BottomRight.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = BottomLeft.x;
		Line.Pos2.posY = BottomLeft.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}
}

void Eclipse::Graphics::IDebugDrawer::DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	float segmentSize = Math::pi2 / aCircleResolution;
	for (int i = 0; i < aCircleResolution - 1; i++)
	{
		Math::Vector2f LeftSegment = aPosition + Math::Vector2f(cos(segmentSize * i), sin((segmentSize * i))) * aRadius * 0.5f;
		Math::Vector2f RightSegment = aPosition + Math::Vector2f(cos(segmentSize * (i + 1)), sin(segmentSize * (i + 1))) * aRadius * 0.5f;

		Line Line;
		Line.Pos1.posX = LeftSegment.x;
		Line.Pos1.posY = LeftSegment.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = RightSegment.x;
		Line.Pos2.posY = RightSegment.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}

	Math::Vector2f LeftSegment = aPosition + Math::Vector2f(cos(segmentSize * (aCircleResolution - 1)), sin((segmentSize * (aCircleResolution - 1)))) * aRadius * 0.5f;
	Math::Vector2f RightSegment = aPosition + Math::Vector2f(cos(0), sin(0)) * aRadius * 0.5f;

	Line Line;
	Line.Pos1.posX = LeftSegment.x;
	Line.Pos1.posY = LeftSegment.y;
	Line.Pos1.color = aColor;

	Line.Pos2.posX = RightSegment.x;
	Line.Pos2.posY = RightSegment.y;
	Line.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(Line);
}

void Eclipse::Graphics::IDebugDrawer::DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution, const Math::Color& aColor)
{
	IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

	float segmentSize = Math::pi2 / aCircleResolution;
	unsigned segmentCount = static_cast<unsigned>(static_cast<float>(aCircleResolution) * 0.5f);
	float offsetRotation = std::atan2f(aDirection.y, aDirection.x) + segmentSize * 1.05f - Math::piHalf;
	for (int i = 0; i < segmentCount - 1; i++)
	{
		Math::Vector2f LeftSegment = aPosition + Math::Vector2f(cos(segmentSize * i + offsetRotation), sin(segmentSize * i + offsetRotation)) * aRadius * 0.5f;
		Math::Vector2f RightSegment = aPosition + Math::Vector2f(cos(segmentSize * (i + 1) + offsetRotation), sin(segmentSize * (i + 1) + offsetRotation)) * aRadius * 0.5f;
		
		Line Line;
		Line.Pos1.posX = LeftSegment.x;
		Line.Pos1.posY = LeftSegment.y;
		Line.Pos1.color = aColor;

		Line.Pos2.posX = RightSegment.x;
		Line.Pos2.posY = RightSegment.y;
		Line.Pos2.color = aColor;

		debugDrawer->myLineCollection.push_back(Line);
	}

	Math::Vector2f LeftSegment = aPosition + Math::Vector2f(cos(segmentSize * (aCircleResolution - 1) + offsetRotation), sin(segmentSize * (aCircleResolution - 1) + offsetRotation)) * aRadius * 0.5f;
	Math::Vector2f RightSegment = aPosition + Math::Vector2f(cos(offsetRotation), sin(offsetRotation)) * aRadius * 0.5f;

	Line Line;
	Line.Pos1.posX = LeftSegment.x;
	Line.Pos1.posY = LeftSegment.y;
	Line.Pos1.color = aColor;

	Line.Pos2.posX = RightSegment.x;
	Line.Pos2.posY = RightSegment.y;
	Line.Pos2.color = aColor;

	debugDrawer->myLineCollection.push_back(Line);
}



void Eclipse::Graphics::IDebugDrawer::BeginRender(const Line& line)
{
	//for (int i = 0; i < line.linePoints.size(); i++)
	//{
	//	LineVTX vert;

	//	std::memcpy(&vert, &line.linePoints[i], sizeof(LineVTX));

	//	vertices.emplace_back(vert);
	//	indices.emplace_back(i);
	//}

	//Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
	//buffer->SetOrCreateBuffer<CameraBuffer>(0);
}


void Eclipse::Graphics::IDebugDrawer::EndRender()
{
	indices.clear();
	//vertices.clear();
}