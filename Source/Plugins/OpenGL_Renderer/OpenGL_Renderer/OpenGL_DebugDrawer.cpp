#include "OpenGL_DebugDrawer.h"

#include "OpenGL/glad/glad.h"

#include "Core/MainSingleton.h"
#include "Core/Math/Vector/Vector2.h"

#include "Renderer/RenderCommands/CommandList.h"


namespace Eclipse::Graphics::OpenGL
{
    static const char* vtxShaderSource =
        "#version 460 core\n"
        "layout(location = 0)in vec2 VertexPosition;\n"
        "layout(location = 1)in vec2 trash;\n"
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
        "}\n";

    static const char* pixelShaderSource =
        "#version 460 core\n"
        "out vec4 frag_colour;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
        "frag_colour = color;\n"
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
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Math::Vector2f), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Math::Vector2f), (void*)0);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
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
        MainSingleton::GetPointer<CommandListManager>()->GetDebugDrawCommandList().Enqueue([&]() {
            glUseProgram(programID);
            });

        for (Line& line : myLineCollection)
        {
            MainSingleton::GetPointer<CommandListManager>()->GetDebugDrawCommandList().Enqueue([&, line]() {
                BeginRender(line);

                glBindBuffer(GL_ARRAY_BUFFER, myVTXbuffer);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LineVTX), vertices.data(), GL_DYNAMIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIDXbuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);

                glBindVertexArray(myLineBuffer);

                glUniform4f(0, line.color.r, line.color.g, line.color.b, line.color.a);

                glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

                EndRender();

                });
        }
    }
}