#pragma once

#include "Editor/Views/IEditorView.h"

#include "Core/Math/Vector/Vector2.h"

#include "Renderer/FrameBuffer.h"

namespace Eclipse::Editor
{
	class GameView : public EditorView<GameView>
	{
		BASIC_VIEW("Game")

		struct Resolution
		{
			Resolution(const char* aName, Math::Vector2f Resolution) : Name(aName), AspectRatioResolution(Resolution.x / Resolution.y, Resolution.y / Resolution.x) {}

			std::string Name;
			Math::Vector2f AspectRatioResolution;
		};

	public:
		void OnOpen() override;
		void Draw() override;

	private:
		void CheckNChangeSceneImageDimension();
		void PopulateDefaultResolutions();
		void DrawFixedResolution();

		void MenuBar();

		void InitSceneBuffer();

		void SetBuffers();

	private:


		Math::Vector2f myWindowSize;
		Math::Vector2f myLastWindowResolution = { -1, -1 };

		Graphics::FrameBuffer myGameFrameBuffer;

		std::vector<Resolution> myResolutions;

		size_t ActiveResolutionIndex;
		Math::Vector2f ActiveResolutionRatios;
	};
}