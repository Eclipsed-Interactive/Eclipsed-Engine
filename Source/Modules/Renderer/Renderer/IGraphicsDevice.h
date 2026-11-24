#pragma once

#include "Renderer.Core.hpp"
#include <unordered_map>
#include "Sprite.h"
#include "TextSprite.h"
#include "ParticleDrawer.h"

#include "ClearFlags.hpp"

#include "FrameBuffer.h"
#include <span>
#include <Core/Standard/Core/Types.hpp>

namespace Eclipse::Graphics
{
#pragma region MOVE THIS TO SEPARATE FILES. TOFFLA
	enum class BlendMode { Alpha, Additive, Multiply, Screen };
	struct TextureDescription {
		int channels = 0;
		int width = 0;
		int height = 0;
	};

	enum ShaderStage : unsigned char
	{
		Compute,
		Pixel,
		Vertex,
		Geometry
	};
#pragma endregion

	class RENDERER_API IGraphicsDevice
	{
	public:
		virtual void Clear(ClearFlags flags = ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil,
			const Math::Color& color = { 0.f, 0.f, 0.f,0.f },
			float depth = 1.f,
			unsigned char stencil = 0) = 0;

		virtual void BindFrameBuffer(unsigned aFrameBuffer) = 0;
		virtual void SetBlendMode(BlendMode mode) = 0;
		virtual FrameBuffer CreateFrameBuffer(bool aFloatFormat = false) = 0;

		virtual void SetViewport(const Math::Vector2ui& size, const Math::Vector2ui& pos = {0, 0}) = 0;

		virtual void ChangeImageDimensions(Math::Vector2f size, bool aFloatFormat = false) = 0;

		virtual Math::Vector4ui ReadPixelOnFrameBuffer(int aFrameBuffer, const Math::Vector2ui& Position, const Math::Vector2f& Size = { 1, 1 }) = 0;

	public:
		virtual Sprite* CreateSprite() = 0;
		virtual TextSprite* CreateTextSprite() = 0;
		virtual ParticleDrawer* CreateParticleDrawer() = 0;

		virtual unsigned int CreateTexture(const std::span<const Byte> somePixels, TextureDescription& aTextureDescription) = 0;
		virtual unsigned int CreateShader(const std::span<const Eclipse::Byte> someSpirvs, ShaderStage stage) = 0;
		virtual unsigned int CreateShaderProgram(unsigned int* ps, unsigned int* vs) = 0;

	public:
		virtual void BindTexture(unsigned textureIndex) = 0;
		virtual void BindTexture(unsigned slot, unsigned textureIndex) = 0;
		//virtual void BindTexture(unsigned slot, Assets::Texture& texture) = 0;
		virtual void BindShader(unsigned id) = 0;
		//virtual void BindMaterial(Assets::Material& material) = 0;
	};
}