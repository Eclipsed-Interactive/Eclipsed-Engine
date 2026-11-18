#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "Renderer.Core.hpp"

#include <Core/GraphicsBuffers/CameraBuffer.h>
#include <Core/GraphicsBuffers/EditorBuffer.h>
#include <Core/GraphicsBuffers/CanvasBuffer.h>
#include <Core/GraphicsBuffers/EmitterParticlesBuffer.h>
#include <Core/GraphicsBuffers/LightBuffer.h>
#include <Core/GraphicsBuffers/MaterialBuffer.h>
#include <Core/GraphicsBuffers/ParticleSpawnBuffer.h>
#include <Core/GraphicsBuffers/SpriteBuffer.h>
#include <Core/GraphicsBuffers/SharedGPUParticlesBuffer.h>
#include <Core/GraphicsBuffers/TransformBuffer.h>
#include <Core/GraphicsBuffers/TextMaterialBuffer.h>
#include <Core/GraphicsBuffers/TextBuffer.h>

namespace Eclipse::Graphics
{
    enum GraphicsBufferEnum
    {
        g_CameraBuffer,
        g_CanvasBuffer,
        g_EditorBuffer,
        g_ConeEmitterDataBuffer,
        g_PointLightBuffer,
        g_SpotLightBuffer,
        g_DirectionalLightBuffer,
        g_AmbientLightBuffer,
        g_MaterialBuffer,
        g_ParticleSpawnBuffer,
        g_SharedGPUParticlesBuffer,
        g_SpriteBuffer,
        g_TextBuffer,
        g_TextMaterialBuffer,
        g_TransformBuffer,

        Count
    };

    class RENDERER_API IGraphicsBuffer
    {
        struct BufferStorage
        {
            void* Data;
            int Size;
        };

    public:

        IGraphicsBuffer() = default;
        virtual ~IGraphicsBuffer() = default;

        void Init();

        template <typename T>
        void GetBuffer(GraphicsBufferEnum bufferEnum, T*& aBuffer);
        
        void SetOrCreateBuffer(GraphicsBufferEnum bufferEnum, int aBufferIndex, void* aBuffer);
        void SetOrCreateBuffer(GraphicsBufferEnum bufferEnum, int aBufferIndex);


    private:
        virtual void AddInternalBuffer(GraphicsBufferEnum aGraphicsBufferEnum, size_t bufferSize) = 0;
        virtual void SetInternal(int aBufferIndex, GraphicsBufferEnum aGraphicsBufferEnum, void* aBuffer, size_t bufferSize) = 0;

        template <typename T>
        void AddBufferInternaly(GraphicsBufferEnum aGraphicsBufferEnum);

    private:
        std::array<BufferStorage, Count> myBufferStorage;
    };

    template <typename T>
    inline void IGraphicsBuffer::GetBuffer(GraphicsBufferEnum bufferEnum, T*& aBuffer)
    {
        aBuffer = static_cast<T*>(myBufferStorage[bufferEnum].Data);
    }

    inline void IGraphicsBuffer::SetOrCreateBuffer(GraphicsBufferEnum bufferEnum, int aBufferIndex, void* aBuffer)
    {
        BufferStorage& bufferStorage = myBufferStorage[bufferEnum];

        memcpy(bufferStorage.Data, aBuffer, bufferStorage.Size);

        SetInternal(aBufferIndex, bufferEnum, bufferStorage.Data, bufferStorage.Size);
    }

    inline void IGraphicsBuffer::SetOrCreateBuffer(GraphicsBufferEnum bufferEnum, int aBufferIndex)
    {
        BufferStorage& bufferStorage = myBufferStorage[bufferEnum];

        SetInternal(aBufferIndex, bufferEnum, bufferStorage.Data, bufferStorage.Size);
    }

    inline void IGraphicsBuffer::Init()
    {
        AddBufferInternaly<CameraBuffer>(g_CameraBuffer);
        AddBufferInternaly<CanvasBuffer>(g_CanvasBuffer);
        AddBufferInternaly<EditorBuffer>(g_EditorBuffer);
        AddBufferInternaly<ConeEmitterDataBuffer>(g_ConeEmitterDataBuffer);
        AddBufferInternaly<PointLightBuffer>(g_PointLightBuffer);
        AddBufferInternaly<SpotLightBuffer>(g_SpotLightBuffer);
        AddBufferInternaly<DirectionalLightBuffer>(g_DirectionalLightBuffer);
        AddBufferInternaly<AmbientLightBuffer>(g_AmbientLightBuffer);
        AddBufferInternaly<MaterialBuffer>(g_MaterialBuffer);
        AddBufferInternaly<ParticleSpawnBuffer>(g_ParticleSpawnBuffer);
        AddBufferInternaly<SharedGPUParticlesBuffer>(g_SharedGPUParticlesBuffer);
        AddBufferInternaly<SpriteBuffer>(g_SpriteBuffer);
        AddBufferInternaly<TextBuffer>(g_TextBuffer);
        AddBufferInternaly<TextMaterialBuffer>(g_TextMaterialBuffer);
        AddBufferInternaly<TransformBuffer>(g_TransformBuffer);
    }

    template <typename T>
    inline void IGraphicsBuffer::AddBufferInternaly(GraphicsBufferEnum aGraphicsBufferEnum)
    {
        myBufferStorage[aGraphicsBufferEnum].Data = new T();
        myBufferStorage[aGraphicsBufferEnum].Size = sizeof(T);

        AddInternalBuffer(aGraphicsBufferEnum, sizeof(T));
    }
}
