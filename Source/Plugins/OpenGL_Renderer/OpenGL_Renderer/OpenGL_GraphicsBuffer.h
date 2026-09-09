#pragma once

#include <unordered_map>
#include "Renderer/IGraphicsBuffer.h"

namespace Eclipse::Graphics
{
    class OpenGL_GraphicsBuffer : public IGraphicsBuffer
    {
    public:
        void AddInternalBuffer(std::type_index aTypeInfo, size_t bufferSize) override;
        void SetInternal(int aBufferIndex, std::type_index aTypeInfo, void* aBuffer, size_t bufferSize) override;

    private:
        std::unordered_map<std::type_index, unsigned> myUniformBufferObjects;
    };   
}