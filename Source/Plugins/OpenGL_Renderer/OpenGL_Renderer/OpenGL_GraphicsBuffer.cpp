#include "OpenGL_GraphicsBuffer.h"

#include "OpenGL/glad/glad.h"
#include <typeinfo>
#include <typeindex>

namespace Eclipse::Graphics
{
    void OpenGL_GraphicsBuffer::SetInternal(int aBufferIndex, GraphicsBufferEnum aGraphicsBufferEnum, void* aBuffer, size_t bufferSize)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, aBufferIndex, myUniformBufferObjects[aGraphicsBufferEnum]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(bufferSize), aBuffer);
    }

    void OpenGL_GraphicsBuffer::AddInternalBuffer(GraphicsBufferEnum aGraphicsBufferEnum, size_t bufferSize)
    {
        unsigned& buffer = myUniformBufferObjects[aGraphicsBufferEnum];
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(bufferSize), nullptr, GL_DYNAMIC_DRAW);
    }

}