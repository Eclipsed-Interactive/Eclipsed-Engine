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


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

	void OpenGL_GraphicsDevice::ChangeImageDimensions(Math::Vector2f size)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	Math::Vector4ui OpenGL_GraphicsDevice::ReadPixelOnFrameBuffer(int aFrameBuffer, const Math::Vector2ui& Position, const Math::Vector2f& Size)
	{
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, aFrameBuffer);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		glReadPixels(Position.x, Position.y, Size.x, Size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

		return Math::Vector4ui(data[0], data[1], data[2], data[3]);
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

	static const char* particleVTXShaderSource =
		"#version 460 core\n"
		"layout(location = 0)in vec2 Position;\n"
		"layout(location = 1)in vec2 Scale;\n"
		"layout(location = 2)in float Rotation;\n"
		"layout(location = 3)in float Trash;\n"

		"layout(std140,binding=0) uniform CameraBuffer\n"
		"{\n"
		"vec2 cameraPosition;\n"
		"vec2 cameraScale;\n"
		"float cameraRotation;\n\n"
		"float resolutionRatio;\n"
		"};\n"

		"out vec2 vScale;\n"
		"out float vRotation;\n"
		"out vec2 vCameraScale;\n"
		"out float vResolutionRatio;\n"

		"void main()\n"
		"{\n"
		"vScale = Scale;\n"
		"vRotation = cameraRotation + Rotation;\n"
		"vCameraScale = cameraScale;\n"
		"vResolutionRatio = resolutionRatio;\n"

		"vec2 vtxPos = (Position - cameraPosition) * cameraScale;\n"
		"vtxPos.x *= resolutionRatio;\n"
		"gl_Position = vec4(vtxPos, 0, 1);\n"
		"}\n";

	static const char* particleGeometryShaderSource =
		"#version 460 core\n"
		"layout(points) in;\n"
		"layout(triangle_strip, max_vertices = 4) out;\n"

		"in vec2 vScale[];\n"
		"in float vRotation[];\n"
		"in vec2 vCameraScale[];\n"
		"in float vResolutionRatio[];\n"

		"void main()\n"
		"{\n"
		"float c = cos(vRotation[0]);\n"
		"float s = sin(vRotation[0]);\n"
		"mat2 rot = mat2(c, s, -s, c);\n"

		"vec2 halfSize = vScale[0] * 0.5;\n"
		"vec2 corners[4] = vec2[4](vec2(-halfSize.x, halfSize.y), vec2(-halfSize.x, -halfSize.y), vec2(halfSize.x, halfSize.y), vec2(halfSize.x, -halfSize.y));\n"

		"for (int i = 0; i < 4; ++i)\n"
		"{\n"
		"vec2 offset = rot * corners[i] * vCameraScale[0];\n"
		"offset.x *= vResolutionRatio[0];\n"
		"gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0, 0.0);\n"
		"EmitVertex();\n"
		"}\n"
		"EndPrimitive();\n"
		"}\n";

	static const char* particlePixelShaderSource =
		"#version 460 core\n"
		"out vec4 frag_colour;\n"
		"void main()\n"
		"{\n"
		"frag_colour = vec4(1, 1, 1, 1);\n"
		"}\n";

	ParticleDrawer* OpenGL_GraphicsDevice::CreateParticleDrawer()
	{
		ParticleDrawer* particleDrawer = new ParticleDrawer;

		glGenVertexArrays(1, &particleDrawer->GetBufferID());
		glBindVertexArray(particleDrawer->GetBufferID());

		glGenBuffers(1, &particleDrawer->GetVertexBufferID());
		glBindBuffer(GL_ARRAY_BUFFER, particleDrawer->GetVertexBufferID());

		{
			const GLsizei stride = sizeof(ParticleData);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(ParticleData, Position));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(ParticleData, Scale));
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(ParticleData, Rotation));
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(ParticleData, Trash));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
		}

		unsigned& VtxShaderID = particleDrawer->GetVTXShaderID();
		unsigned& GeoShaderID = particleDrawer->GetGeometryShaderID();
		unsigned& PxShaderID = particleDrawer->GetPixelShaderID();
		unsigned& ProgramID = particleDrawer->GetProgramID();

		auto checkShader = [](GLuint id, const char* name)
			{
				GLint ok = GL_FALSE;
				glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
				if (!ok)
				{
					GLint len = 0;
					glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
					std::string log(len > 1 ? len : 1, '\0');
					glGetShaderInfoLog(id, len, NULL, log.data());
					fprintf(stderr, "%s shader compile failed:\n%s\n", name, log.c_str());
					
					throw;
				}
			};

		VtxShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VtxShaderID, 1, &particleVTXShaderSource, NULL);
		glCompileShader(VtxShaderID);
		checkShader(VtxShaderID, "Vertex");

		GeoShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GeoShaderID, 1, &particleGeometryShaderSource, NULL);
		glCompileShader(GeoShaderID);
		checkShader(GeoShaderID, "Geometry");

		PxShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(PxShaderID, 1, &particlePixelShaderSource, NULL);
		glCompileShader(PxShaderID);
		checkShader(PxShaderID, "Fragment");

		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VtxShaderID);
		glAttachShader(ProgramID, GeoShaderID);
		glAttachShader(ProgramID, PxShaderID);
		glLinkProgram(ProgramID);

		GLint linked = GL_FALSE;
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLint len = 0;
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &len);
			std::string log(len > 1 ? len : 1, '\0');
			glGetProgramInfoLog(ProgramID, len, NULL, log.data());
			fprintf(stderr, "Program link failed:\n%s\n", log.c_str());
			throw;
		}


		glBindVertexArray(0);

		return particleDrawer;
	}

	void OpenGL_GraphicsDevice::BindTexture(unsigned textureIndex)
	{
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}

	void OpenGL_GraphicsDevice::BindTexture(unsigned slot, unsigned textureIndex)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
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
		if(texture.dataPtr)
			BindTexture(0, texture); // Change to albedo slot
	}
}

