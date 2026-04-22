#pragma once

#include <unordered_map>
#include "GraphicsEngine/GraphicsBuffer/BaseGraphicsBuffer.h"


namespace Eclipse
{
    class OpenGLGraphicsBuffer : public BaseGraphicsBuffer
    {
    public:
        void AddInternalBuffer(std::type_index aTypeInfo, size_t bufferSize) override;
        void SetInternal(int aBufferIndex, std::type_index aTypeInfo, void* aBuffer, size_t bufferSize) override;

    private:
        std::unordered_map<std::type_index, unsigned> myUniformBufferObjects;
    };   
}