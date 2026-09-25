#pragma once

#include "Editor/Views/IEditorView.h"
#include "Editor/Views/BaseRenderingView.h"

#include "Renderer/FrameBuffer.h"

#include "Core/Math/Color.h"

#include "Core/Math/Vector/Vector2.h"

namespace Eclipse
{
	class Component;
	class Transform2D;
	class SpriteRenderer2D;
}

namespace Eclipse::Editor
{
	class SceneView : public EditorView<SceneView>, BaseRenderingView
	{
		BASIC_VIEW("Scene")

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

		Eclipse::Math::Vector2ui GetSceneViewMousePosition();

	public:
		void Draw() override;
		void OnOpen() override;

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

		Graphics::FrameBuffer sceneBuffer;
		Math::Color ClearColor = { 0.2f, 0.2f, 0.2f, 1.f };

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
		bool AlwaysRenderSpriteSelector = true;
	};
}