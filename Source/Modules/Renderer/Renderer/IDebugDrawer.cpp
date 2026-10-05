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

    Line* line = nullptr;
    if (!aUsePrev)
    {
        line = &debugDrawer->myLineCollection.emplace_back();
        line->color = aColor;
    }
    else
        line = &debugDrawer->myLineCollection.back();


    line->linePoints.emplace_back(aStart);
    line->linePoints.emplace_back(aEnd);
}

void Eclipse::Graphics::IDebugDrawer::DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    line.linePoints.emplace_back(aStartPos);
    line.linePoints.emplace_back(aStartPos + aDirection);
}

void Eclipse::Graphics::IDebugDrawer::DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength, float anArrowSpan, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    aDirection.Normalize();

    Math::Vector2f endPosition = aStartPos + aDirection * aLineLength;

    line.linePoints.emplace_back(aStartPos);
    line.linePoints.emplace_back(endPosition);

    Math::Vector2f arrowCornersStart = endPosition - aDirection * anArrowSpan;

    Math::Vector2f rightVector = Math::Vector2f(aDirection.y, -aDirection.x);

    Math::Vector2f rightArrowCorner = arrowCornersStart + rightVector * anArrowSpan;
    Math::Vector2f leftArrowCorner = arrowCornersStart - rightVector * anArrowSpan;

    line.linePoints.emplace_back(endPosition);
    line.linePoints.emplace_back(leftArrowCorner);

    line.linePoints.emplace_back(endPosition);
    line.linePoints.emplace_back(rightArrowCorner);
}

void Eclipse::Graphics::IDebugDrawer::DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    line.linePoints.emplace_back(aPosition - aHalfExtents);
    line.linePoints.emplace_back(aPosition + Math::Vector2f{ -aHalfExtents.x, aHalfExtents.y });

    line.linePoints.emplace_back(aPosition + Math::Vector2f{ -aHalfExtents.x, aHalfExtents.y });
    line.linePoints.emplace_back(aPosition + aHalfExtents);

    line.linePoints.emplace_back(aPosition + aHalfExtents);
    line.linePoints.emplace_back(aPosition + Math::Vector2f{ aHalfExtents.x, -aHalfExtents.y });

    line.linePoints.emplace_back(aPosition + Math::Vector2f{ aHalfExtents.x, -aHalfExtents.y });
    line.linePoints.emplace_back(aPosition - aHalfExtents);

    for (auto& point : line.linePoints)
    {
        Math::Vector2f pivot = aPosition;

        float cosTheta = cos(aRotation);
        float sinTheta = sin(aRotation);

        float dx = point.x - pivot.x;
        float dy = point.y - pivot.y;

        float rotatedX = dx * cosTheta - dy * sinTheta;
        float rotatedY = dx * sinTheta + dy * cosTheta;

        point.x = rotatedX + pivot.x;
        point.y = rotatedY + pivot.y;
    }
}

void Eclipse::Graphics::IDebugDrawer::DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    line.linePoints.emplace_back(aMinPosition);
    line.linePoints.emplace_back(Math::Vector2f{ aMinPosition.x, aMaxPosition.y });

    line.linePoints.emplace_back(Math::Vector2f{ aMinPosition.x, aMaxPosition.y });
    line.linePoints.emplace_back(aMaxPosition);

    line.linePoints.emplace_back(aMaxPosition);
    line.linePoints.emplace_back(Math::Vector2f{ aMaxPosition.x, aMinPosition.y });

    line.linePoints.emplace_back(Math::Vector2f{ aMaxPosition.x, aMinPosition.y });
    line.linePoints.emplace_back(aMinPosition);
}

void Eclipse::Graphics::IDebugDrawer::DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    //float resRatio = 0.f;// TemporarySettingsSingleton::Get().GetResolutionRatio();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    float segmentSize = Math::pi2 / aCircleResolution;

    for (int i = 0; i < aCircleResolution - 1; i++)
    {
        line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * i), sin((segmentSize * i))) * aRadius * 0.5f);
        line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * (i + 1)), sin(segmentSize * (i + 1))) * aRadius * 0.5f);
    }

    line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * (aCircleResolution - 1)), sin((segmentSize * (aCircleResolution - 1)))) * aRadius * 0.5f);
    line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(0), sin(0)) * aRadius * 0.5f);
}

void Eclipse::Graphics::IDebugDrawer::DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution, const Math::Color& aColor)
{
    IDebugDrawer* debugDrawer = MainSingleton::GetPointer<IRenderer>()->GetDebugDrawer();

    //float resRatio = 0.f;//TemporarySettingsSingleton::Get().GetResolutionRatio();

    Line& line = debugDrawer->myLineCollection.emplace_back();
    line.color = aColor;

    float segmentSize = Math::pi2 / aCircleResolution;

    unsigned segmentCount = static_cast<unsigned>(static_cast<float>(aCircleResolution) * 0.5f);

    float offsetRotation = std::atan2f(aDirection.y, aDirection.x) + segmentSize * 1.05f - Math::piHalf;

    for (int i = 0; i < segmentCount - 1; i++)
    {
        line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * i + offsetRotation), sin(segmentSize * i + offsetRotation)) * aRadius * 0.5f);
        line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * (i + 1) + offsetRotation), sin(segmentSize * (i + 1) + offsetRotation)) * aRadius * 0.5f);
    }

    line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(segmentSize * (aCircleResolution - 1) + offsetRotation), sin(segmentSize * (aCircleResolution - 1) + offsetRotation)) * aRadius * 0.5f);
    line.linePoints.emplace_back(aPosition + Math::Vector2f(cos(offsetRotation), sin(offsetRotation)) * aRadius * 0.5f);
}



void Eclipse::Graphics::IDebugDrawer::BeginRender(const Line& line)
{
    for (int i = 0; i < line.linePoints.size(); i++)
    {
        //auto& temporarySingleton = TemporarySettingsSingleton::Get();

        //float oneDivResX = temporarySingleton.GetOneDivResolutionX();
        //float oneDivResY = temporarySingleton.GetOneDivResolutionY();

        LineVTX vert;

        std::memcpy(&vert, &line.linePoints[i], sizeof(LineVTX));

        // vert.trashDataX = line.linePoints[i].x;// * oneDivResX;
        // vert.trashDataY = line.linePoints[i].y;// * oneDivResY;

        vertices.emplace_back(vert);
        indices.emplace_back(i);
    }

    Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
    buffer->SetOrCreateBuffer<CameraBuffer>(0);
}


void Eclipse::Graphics::IDebugDrawer::EndRender()
{
    indices.clear();
    vertices.clear();
}