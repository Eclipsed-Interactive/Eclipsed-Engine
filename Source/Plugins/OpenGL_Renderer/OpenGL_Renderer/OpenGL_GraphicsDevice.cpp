#include "OpenGL_GraphicsDevice.h"

#include "OpenGL/glad/glad.h"

namespace Eclipse::Graphics::OpenGL
{
	void OpenGL_GraphicsDevice::Clear(ClearFlags flags, const Math::Color& color, float depth, unsigned char stencil)
	{
		GLbitfield mask = 0;

		if ((flags & ClearFlags::Color) != ClearFlags::None)
		{
			glClearColor(color.r, color.g, color.b, color.a);
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if ((flags & ClearFlags::Depth) != ClearFlags::None)
		{
			glClearDepth(depth);
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if ((flags & ClearFlags::Stencil) != ClearFlags::None)
		{
			glClearStencil(stencil);
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(mask);
	}

	void OpenGL_GraphicsDevice::BindFrameBuffer(unsigned aFrameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);
	}

	FrameBuffer OpenGL_GraphicsDevice::CreateFrameBuffer()
	{
		FrameBuffer buf;

		glGenFramebuffers(1, &buf.frameBufferIndex);
		glBindFramebuffer(GL_FRAMEBUFFER, buf.frameBufferIndex);

		glGenTextures(1, &buf.textureIndex);
		glBindTexture(GL_TEXTURE_2D, buf.textureIndex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buf.textureIndex, 0);

		return buf;
	}

	void OpenGL_GraphicsDevice::SetViewport(const Math::Vector2ui& size, const Math::Vector2ui& pos)
	{
		glViewport(pos.x, pos.y, size.x, size.y);
	}

	Sprite* OpenGL_GraphicsDevice::CreateSprite()
	{
		Sprite* sprite = new Sprite;

		glGenVertexArrays(1, &sprite->GetSpriteBufferID());
		glBindVertexArray(sprite->GetSpriteBufferID());

		glGenBuffers(1, &sprite->GetVertexBufferID());
		glGenBuffers(1, &sprite->GetIndexBufferID());

		glBindBuffer(GL_ARRAY_BUFFER, sprite->GetVertexBufferID());
		glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->Verticies), sprite->Verticies, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->GetIndexBufferID());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite->Indices), sprite->Indices, GL_STATIC_DRAW);

		{
			// glVertexAttribPointer(shader layout index, total number of floats/ints/bools etc, GL_[your type], GL_FALSE, vertex stride, offset from last layout position start)
			// Matrix4x4 would be index 0, 1, 2 and 3 set as in setting 4 float4's

			size_t offset = 0;
			// Position
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void*)offset);
			offset += 2 * sizeof(float);

			// TexCoord
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void*)offset);
			offset += 2 * sizeof(float);

			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(0);
		}

		glBindVertexArray(0);

		return sprite;
	}

	TextSprite* OpenGL_GraphicsDevice::CreateTextSprite()
	{
		TextSprite* sprite = new TextSprite;
		glGenVertexArrays(1, &sprite->GetSpriteBufferID());
		glBindVertexArray(sprite->GetSpriteBufferID());

		glGenBuffers(1, &sprite->GetVertexBufferID());
		glGenBuffers(1, &sprite->GetIndexBufferID());

		glBindBuffer(GL_ARRAY_BUFFER, sprite->GetVertexBufferID());
		glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->Vertices), sprite->Vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->GetIndexBufferID());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite->Indices), sprite->Indices, GL_STATIC_DRAW);

		{
			// glVertexAttribPointer(shader layout index, total number of floats/ints/bools etc, GL_[your type], GL_FALSE, vertex stride, offset from last layout position start)
			// Matrix4x4 would be index 0, 1, 2 and 3 set as in setting 4 float4's

			size_t offset = 0;
			// Position
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextSprite::Vertex), (void*)offset);
			offset += 2 * sizeof(float);

			// TexCoord
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextSprite::Vertex), (void*)offset);
			offset += 2 * sizeof(float);

			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(0);
		}

		glBindVertexArray(0);

		return sprite;
	}

	void OpenGL_GraphicsDevice::BindTexture(unsigned textureIndex)
	{
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}

	void OpenGL_GraphicsDevice::BindTexture(unsigned slot, Assets::Texture& texture)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	}

	void OpenGL_GraphicsDevice::BindShader(unsigned id)
	{
		glUseProgram(id);
	}

	void OpenGL_GraphicsDevice::BindMaterial(Assets::Material& material)
	{
		//materialBuffer.color = dataPtr->color
		material.GetBuffer().color = material.dataPtr->color;
		BindShader(material.GetProgramID());

		Assets::Texture& texture = material.GetTexture();
		BindTexture(0, texture); // Change to albedo slot
	}
}

