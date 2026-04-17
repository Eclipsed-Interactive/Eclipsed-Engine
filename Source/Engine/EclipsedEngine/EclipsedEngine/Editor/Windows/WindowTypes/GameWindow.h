#pragma once

#include "Window.h"
#include "EclipsedEngine/Editor/Windows/WindowBase.h"


#include "RenderingWindow.h"
#include "CoreEngine/Math/Vector/Vector2.h"

namespace Eclipse::Editor
{
	class GameWindow final : public AbstractWindow, public BaseRenderingWindow
	{
		EDITOR_WINDOW_BASE_2(GameWindow, "Game")

	public:

		void UpdateFreeAspect();
		void UpdateSpecifiedRes();

		void Update() override;
		void Open() override;


		static inline Math::Vector2f myGameImageResolution = { 0,0 };

	private:
		unsigned myGameFrameBuffer = 0;
		unsigned myGameTexture = 0;

		Math::Vector2i myLastWindowResolution = { 0, 0 };

		enum WindowMode
		{
			FreeAspect,
			AnythingElse
		} myCurrentWindowMode;

		int currentResIndex = 1;

		Math::Vector2f myWindowResAspect = {1.7777f, 0.5625f};

		float menuHeight = 0;

		bool myDrawGameGizmos = false;
	};
}