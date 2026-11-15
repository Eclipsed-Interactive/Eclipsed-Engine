//#ifdef ECL_EDITOR
//#pragma once
//
//#include "Editor/Views/IEditorView.h"
//
//#include "RenderingWindow.h"
//
//#include "Core/Math/Vector/Vector2.h"
//#include "Core/Math/Vector/Vector4.h"
//
//#include "Core/Math/RectSizePos.h"
//#include "Assets/Assets/TextureAsset.h"
//
//#include "EclipsedEngine.Core.hpp"
//
//namespace Eclipse::Editor
//{
//	class ECL_API SpriteEditor final : public EditorView<SpriteEditor>
//	{
//		BASIC_VIEW("Sprite Editor");
//
//	public:
//		void OnOpen() override;
//		void Draw() override;
//
//		static void SetActiveTexture(const Assets::GUID& guid);
//
//	private:
//		static void SaveMeta(const Assets::GUID& guid);
//		static void LoadMeta(const Assets::GUID& guid);
//
//		void EdgeScaling();
//
//		void ScrollManager();
//		void MouseManager();
//
//		void DrawRects();
//		void RectProperties();
//
//		bool IsInPropertyWindow();
//		bool CheckIfRectMove(Math::Vector2f aOffsetPositionScaledRight);
//
//		bool RectangleEdgeChoosing(Math::Vector2f aMousePosition);
//		void ChangeEdgeCursor(Math::Vector2f aMousePosition);
//
//	private:
//		Math::Vector2f myInspectorPosition = { 0, 0 };
//		Math::Vector2f myLastInspectorPosition = { 0, 0 };
//		Math::Vector2f myInspectorScale = { 1, 1 };
//
//		Math::Vector2f myInspectorPositionOffsetFromZeroZero = { 0, 0 };
//		Math::Vector2f mySaveMin = { 0, 0 };
//
//	private:
//		static inline Math::Vector2f mySpriteScaleSnapping;
//
//	private:
//		Math::Vector2f myWindowSize;
//		Math::Vector2f myLastWindowResolution = { 0, 0 };
//		Math::Vector2f normalizedMousePosition = { 0, 0 };
//		Math::Vector2i windowRelativeMousePosition = { 0, 0 };
//		Math::Vector2f myStartCursorPosition = { 0, 0 };
//
//		ImVec2 myPropertyWindowPosition = { 0, 0 };
//		ImVec2 mySizeOfProperyWindow = { 0, 0 };
//
//		Math::Vector4f backgroundColor = { 0.7f, 0.7f, 0.7f, 1.f };
//
//		static inline Math::Vector2i myTextureSize;
//		float myTextureSizeAspectRatio = 0;
//
//	private:
//		bool holdingRect = false;
//
//		float totalYScroll = 0;
//		float lastScroll = 0;
//		bool mouseIsDown = false;
//		bool mouseIsDownLeft = false;
//
//		Math::Vector2f mySpriteCurrentMin;
//		Math::Vector2f mySpriteCurrentMax;
//
//		float currentPositioningX = 0.f;
//		float currentScalingX = 0.f;
//		float* myScaleEdgeScalingX = nullptr;
//		float* myPositionEdgeScalingX = nullptr;
//
//		float currentPositioningY = 0.f;
//		float currentScalingY = 0.f;
//		float* myScaleEdgeScalingY = nullptr;
//		float* myPositionEdgeScalingY = nullptr;
//
//		float myMinEdgeScaling;
//		float myMaxEdgeScaling;
//
//		float mouseEdgeSensetivity = 5;
//
//	private:
//		static inline Assets::Texture myTexture;
//		static inline bool textureSet;
//		static inline Assets::GUID myActiveAsset;
//
//
//		struct Rect
//		{
//			Math::RectSizePos rect;
//
//			std::string name;
//			bool isSelected = false;
//		};
//
//
//		Math::Vector2f cursorPosBeforeImage = { 0, 0 };
//
//		Rect myCopiedRectInfo;
//
//		Rect* mySelectedRectPtr = nullptr;
//		Math::Vector2f savedPositionBeforeMove;
//		static inline std::vector<Rect> myRects;
//	};
//}
//#endif