#include "OpenGL_DebugDrawer.h"

#include "OpenGL/glad/glad.h"

#include "Core/MainSingleton.h"
#include "Core/Math/Vector/Vector2.h"

#include "Renderer/RenderCommands/CommandList.h"

#include "Renderer/IGraphicsBuffer.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RendererManager.h"
#include "Core/GraphicsBuffers/CameraBuffer.h"


namespace Eclipse::Graphics::OpenGL
{
	static const char* vtxShaderSource =
		"#version 460 core\n"
		"layout(location = 0)in vec4 VTXColor;\n"
		"layout(location = 1)in vec2 VertexPosition;\n"
		"layout(location = 2)in vec2 trash;\n"

		"out vec4 Color;\n"

		"layout(std140,binding=0) uniform CameraBuffer\n"
		"{\n"

		"vec2 cameraPosition;\n"
		"vec2 cameraScale;\n"
		"float cameraRotation;\n\n"
		"float resolutionRatio;\n"

		"};\n"
		"void main()\n"
		"{\n"
		"mat2 rotationMatrix = mat2(cos(cameraRotation), -sin(cameraRotation), sin(cameraRotation), cos(cameraRotation));\n"
		"vec2 vtxPos = VertexPosition;\n"
		//"vec2 vtxPos = VertexPosition * 2 - 1;\n"
		"vtxPos = vtxPos * rotationMatrix;\n"
		"vtxPos -= cameraPosition;\n"
		"vtxPos.x *= resolutionRatio;\n"
		"gl_Position = vec4(vtxPos * cameraScale, 0, 1);\n"
		"Color = VTXColor;\n"
		"}\n";

	static const char* pixelShaderSource =
		"#version 460 core\n"
		"out vec4 frag_colour;\n"
		"in vec4 Color;\n"
		"void main()\n"
		"{\n"
		"frag_colour = Color;\n"
		"}\n";


	void OpenGL_DebugDrawer::Init()
	{
		glGenVertexArrays(1, &myLineBuffer);
		glBindVertexArray(myLineBuffer);

		glGenBuffers(1, &myVTXbuffer);
		glGenBuffers(1, &myIDXbuffer);

		glBindBuffer(GL_ARRAY_BUFFER, myVTXbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIDXbuffer);

		{
			const GLsizei stride = sizeof(LineVTX);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(LineVTX, color));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(LineVTX, posX));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(LineVTX, TrashX));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}

		vtxShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vtxShaderID, 1, &vtxShaderSource, NULL);
		glCompileShader(vtxShaderID);

		pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(pixelShaderID, 1, &pixelShaderSource, NULL);
		glCompileShader(pixelShaderID);

		programID = glCreateProgram();
		glAttachShader(programID, vtxShaderID);
		glAttachShader(programID, pixelShaderID);
		glLinkProgram(programID);
	}

	void OpenGL_DebugDrawer::Render()
	{
		MainSingleton::GetPointer<CommandListManager>()->GetDebugDrawCommandList().Enqueue([&]()
			{
				glUseProgram(programID);




				for (int i = 0; i < myLineCollection.size() * 2; i++)
				{
					indices.emplace_back(i);
				}

				Graphics::IGraphicsBuffer* buffer = Graphics::RendererManager::GetRenderer().GetGraphicsBuffer();
				buffer->SetOrCreateBuffer<CameraBuffer>(0);


				//Line line;

				//memcpy(&line, myLineCollection.data(), sizeof(Line))

				glBindBuffer(GL_ARRAY_BUFFER, myVTXbuffer);
				glBufferData(GL_ARRAY_BUFFER, myLineCollection.size() * sizeof(Line), myLineCollection.data(), GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIDXbuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);

				glBindVertexArray(myLineBuffer);

				//glUniform4f(0, line.color.r, line.color.g, line.color.b, line.color.a);

				glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

				EndRender();
			}
		);
	}
}