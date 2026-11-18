#pragma once

#include <unordered_map>
#include "Renderer/IGraphicsBuffer.h"

namespace Eclipse::Graphics
{
    class OpenGL_GraphicsBuffer : public IGraphicsBuffer
    {
    public:
        void AddInternalBuffer(GraphicsBufferEnum aGraphicsBufferEnum, size_t bufferSize) override;
        void SetInternal(int aBufferIndex, GraphicsBufferEnum aGraphicsBufferEnum, void* aBuffer, size_t bufferSize) override;

    private:
        std::array<unsigned, Count> myUniformBufferObjects;
    };   
}