#pragma once

#include "Renderer.Core.hpp"
#include <unordered_map>
#include "Sprite.h"
#include "TextSprite.h"

#include "Assets/Assets/TextureAsset.h"
#include "Assets/Assets/MaterialAsset.h"

#include "ClearFlags.hpp"

#include "FrameBuffer.h"

namespace Eclipse::Graphics
{
	class RENDERER_API IGraphicsDevice
	{
	public:
		virtual void Clear(ClearFlags flags = ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil,
			const Math::Color& color = { 0.f, 0.f, 0.f,0.f },
			float depth = 1.f,
			unsigned char stencil = 0) = 0;

		virtual void BindFrameBuffer(unsigned aFrameBuffer) = 0;

		virtual FrameBuffer CreateFrameBuffer() = 0;

		virtual void SetViewport(const Math::Vector2ui& size, const Math::Vector2ui& pos = {0, 0}) = 0;

	public:
		virtual Sprite* CreateSprite() = 0;
		virtual TextSprite* CreateTextSprite() = 0;

	public:
		virtual void BindTexture(unsigned textureIndex) = 0;
		virtual void BindTexture(unsigned slot, Assets::Texture& texture) = 0;
		virtual void BindShader(unsigned id) = 0;
		virtual void BindMaterial(Assets::Material& material) = 0;
	};
}