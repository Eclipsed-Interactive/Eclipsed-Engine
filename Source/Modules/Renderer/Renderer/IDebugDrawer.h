#pragma once

#include "Sprite.h"
#include "TextSprite.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Color.h"

#include "Renderer.Core.hpp"

namespace Eclipse
{
	class RENDERER_API DebugDrawer
	{
	public:
		static void DrawLine(Math::Vector2f aStart, Math::Vector2f aEnd, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
		static void DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
		static void DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength = 1.f, float anArrowSpan = 0.1f, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

		static void DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
		static void DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

		static void DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
		static void DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
	};

	namespace Graphics
	{
		class RENDERER_API IDebugDrawer
		{
		public:
			void virtual Init() = 0;
			void virtual Render() = 0;

			void Begin();

		private:


			friend class DebugDrawer;

			static void DrawLine(Math::Vector2f aStart, Math::Vector2f aEnd, bool aUsePrev = false, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
			static void DrawRay(Math::Vector2f aStartPos, Math::Vector2f aDirection, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
			static void DrawArrow(Math::Vector2f aStartPos, Math::Vector2f aDirection, float aLineLength = 1.f, float anArrowSpan = 0.1f, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

			static void DrawSquare(Math::Vector2f aPosition, float aRotation, Math::Vector2f aHalfExtents, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
			static void DrawSquareMinMax(Math::Vector2f aMinPosition, Math::Vector2f aMaxPosition, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

			static void DrawCircle(Math::Vector2f aPosition, float aRadius, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));
			static void DrawHalfCircle(Math::Vector2f aPosition, float aRadius, const Math::Vector2f& aDirection, unsigned aCircleResolution = 16, const Math::Color& aColor = Math::Color(0, 1, 0, 1));

		protected:
			struct Line
			{
				std::vector<Math::Vector2f> linePoints;
				Math::Color color;
			};

			void BeginRender(const Line& line);
			void EndRender();


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
		};
	}
}