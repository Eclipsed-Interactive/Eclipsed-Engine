#ifdef ECLIPSED_EDITOR
#pragma once

#include "EclipsedEngine/Editor/Windows/WindowBase.h"
#include "RenderingWindow.h"

#include "CoreEngine/Math/Vector/Vector2.h"

#include <vector>

namespace Eclipse
{
	class Component;
	class Transform2D;
	class SpriteRenderer2D;

	namespace Editor
	{
		class SceneWindow final : public AbstractWindow, BaseRenderingWindow
		{
			EDITOR_WINDOW_BASE_2(SceneWindow, "Scene")

		public:
			static void ZoomToObject(unsigned aObject);
			static void ResetCamera();
			
		private:
			void GizmoManager(Eclipse::Transform2D* aTransform);

			void ObjectSnappingGizmo();

			void SpriteSelector();

			void ScrollManager();
			void MouseManager();
			void SpriteDragging();

		public:
			void Update() override;
			void Open() override;

			void InitSceneBuffer();
			void InitSelectedObjectShader();

		public:
			static inline Math::Vector2f myInspectorPosition = { 0, 0 };
			static inline float myInspectorRotation = 0;
			static inline Math::Vector2f myInspectorScale = { 1, 1 };

		private:
			Math::Vector2f myWindowSize;
			Math::Vector2f myLastWindowResolution = { 0, 0 };
			Math::Vector2f normalizedMousePosition = { 0, 0 };
			Math::Vector2ui windowRelativeMousePosition = { 0, 0 };

		private:
			Math::Vector2f mySpriteMouseDownPosition = { 0, 0 };
			Math::Vector2f mySpriteMoveVector = { 0, 0 };

		private:
			::Eclipse::SpriteRenderer2D* mySelectedObject = nullptr;
			unsigned mySelectedSpriteHighlightProgram;

		private:
			std::vector<std::vector<Component*>> myCopiedComponentsFromObjects;

		private:
			bool myIsSnapping = false;
			float mySnappingDistance = 0.01f;

		private:
			bool myGizmoMoveX = false;
			bool myGizmoMoveY = false;

		private:

			static inline float totalYScroll = 0;
			
			float lastScroll = 0;
			bool mouseIsDown = false;


			bool draggingSprite = false;

		private:
			unsigned mySceneFrameBuffer = 0;
			unsigned mySceneTexture = 0;

			bool AlwaysRenderSpriteSelector = true;
		};
	}
}
#endif