#pragma once

#include "Renderer/IGraphicsDevice.h"

namespace Eclipse::Graphics::OpenGL
{
	class OpenGL_GraphicsDevice : public IGraphicsDevice
	{
	public:
		void Clear(
			ClearFlags flags = ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, 
			const Math::Color& color = {0.f, 0.f, 0.f, 0.f},
			float depth = 1.f,
			unsigned char stencil = 0	
		) override;

		void BindFrameBuffer(unsigned aFrameBuffer) override;

		FrameBuffer CreateFrameBuffer(bool aFloatFormat) override;

		void SetViewport(const Math::Vector2ui& size, const Math::Vector2ui& pos = { 0, 0 }) override;

		void ChangeImageDimensions(Math::Vector2f size, bool aFloatFormat) override;

		Math::Vector4ui ReadPixelOnFrameBuffer(int aFrameBuffer, const Math::Vector2ui& Position, const Math::Vector2f& Size = { 1, 1 }) override;

	public:
		Sprite* CreateSprite() override;
		TextSprite* CreateTextSprite() override;
		ParticleDrawer* CreateParticleDrawer() override;

		void CreateComputeShaderProgram(unsigned& SpawningComputeShaderID, unsigned& SpawningParticleProgram, const char* name);
		void CreateParticleShaderStorageBufferObject(unsigned& ParticleDataSSBO, unsigned& FreeListSSBO);

		void SetBlendMode(BlendMode mode) override;

	public:
		unsigned int CreateTexture(const std::span<const Byte> somePixels, TextureDescription& aTextureDescription) override;
		unsigned int CreateShader(const std::span<const Eclipse::Byte> someSpirvs, ShaderStage aStage) override;
		unsigned int CreateShaderProgram(unsigned int* ps, unsigned int* vs) override;


	public:
		void BindTexture(unsigned textureIndex) override;
		void BindTexture(unsigned slot, unsigned textureIndex) override;
		//void BindTexture(unsigned slot, Assets::Texture& texture) override;
		void BindShader(unsigned id) override;
		//void BindMaterial(Assets::Material& material) override;
	};
}