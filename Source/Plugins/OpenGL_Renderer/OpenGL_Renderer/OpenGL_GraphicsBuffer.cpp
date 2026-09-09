#include "OpenGL_GraphicsBuffer.h"

#include "OpenGL/glad/glad.h"
#include <typeinfo>
#include <typeindex>

namespace Eclipse::Graphics
{
    void OpenGL_GraphicsBuffer::SetInternal(int aBufferIndex, std::type_index aTypeInfo, void* aBuffer, size_t bufferSize)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, aBufferIndex, myUniformBufferObjects[aTypeInfo]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, static_cast<GLsizeiptr>(bufferSize), aBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGL_GraphicsBuffer::AddInternalBuffer(std::type_index aTypeInfo, size_t bufferSize)
    {
        unsigned& buffer = myUniformBufferObjects[aTypeInfo];
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(bufferSize), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

}