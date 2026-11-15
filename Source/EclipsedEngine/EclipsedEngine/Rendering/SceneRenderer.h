#pragma once

#include "Renderer/FrameBuffer.h"
#include <Assets/Assets/MaterialAsset.h>

#include "EclipsedEngine.Core.hpp"

namespace Eclipse::Rendering
{
	class ECL_API SceneRenderer final
	{
	public:
		SceneRenderer() = delete;
		~SceneRenderer() = delete;

	public:
		static void Init();
		static bool Render();

		static void SetResolution(Math::Vector2f resolution);

		[[nodiscard]] static Graphics::FrameBuffer GetFrameBuffer() { return GameFrameBuffer; }

	private:
		static void CreateBuffers();
		static void GetMaterials();

	private:
		static void RenderLights();
		static void RenderSprites();

		static void UpdateCameraBuffer();

	private:
		static inline Math::Vector2f WindowSize;

		static inline Graphics::FrameBuffer GameFrameBuffer;
		static inline Graphics::FrameBuffer PointLightFrameBuffer;
		static inline Graphics::FrameBuffer SpotLightFrameBuffer;

		static inline Assets::Material LightCompositeMat;
	};
}