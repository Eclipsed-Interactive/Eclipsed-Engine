#pragma once

#include <vector>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Color.h"

namespace Eclipse
{
    class DebugDrawer
    {
    public:
        DebugDrawer() = default;
        ~DebugDrawer() = default;

        static DebugDrawer& Get()
        {
            static DebugDrawer instance;
            return instance;
        }

        void Init();
        
        void Begin();
        void Render();

        static void DrawLine(Math::Vector2f aStart, Math::Vector2f aEnd, bool aUsePrev = false, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
        static void DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
        static void DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength = 1.f, float anArrowSpan = 0.1f, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

        static void DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
        static void DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

        static void DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
        static void DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

    private:

        struct Line
        {
            std::vector<Math::Vector2f> linePoints;
            Math::Color color;
        };

        std::vector<Line> myLineCollection;

        struct LineVTX
        {
            float posX;
            float posY;

            // float SecondPosX;
            // float SecondPosY;
        };

        std::vector<LineVTX> vertices;
        std::vector<unsigned> indices;

        unsigned myVTXbuffer;
        unsigned myIDXbuffer;
        unsigned myLineBuffer;

        unsigned pixelShaderID;
        unsigned vtxShaderID;
        unsigned programID;
    };
}
