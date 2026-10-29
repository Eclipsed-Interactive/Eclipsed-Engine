#pragma once

#include "EclipsedEngine/Components/Component.h"

#include "Core/GraphicsBuffers/CameraBuffer.h"
#include "Core/Math/Color.h"

#define GET_AND_SET(GET, SET, VAR) __declspec(property(get = GET, put = SET)) VAR;

namespace Eclipse
{
    class Camera : public Component
    {
        MAKE_REFLECTABLE(Camera);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(Camera, 100)

    public:
        void OnDestroy() override;

        void OnComponentAdded() override;
        void EditorUpdate() override;
        void OnDrawGizmos() override;

    public:
        inline Math::Color GetClearColor() { return myClearColor; }
        inline void SetClearColor(const Math::Color& aClearColor) { myClearColor = aClearColor; }

        inline float GetCameraZoom() { return myCameraZoom; }
        inline void SetCameraZoom(float aCameraZoom) { myCameraZoom = aCameraZoom; }

        inline CameraBuffer& GetBuffer() { return myCameraBuffer; }

    public:
        static inline bool DrawCameraGizmos = true;
        static inline Camera* MainCamera = nullptr;

    private:
        bool created;

        CameraBuffer myCameraBuffer;

        Math::Vector2f minBoundsWorld;
        Math::Vector2f maxBoundsWorld;

    private:
        [[SerializeField, Step(0.01f)]]
        float myCameraZoom = 1.f;

        [[SerializeField]]
        Math::Color myClearColor = { 0.2f, 0.3f, 0.4f, 1 };

    public:
        GET_AND_SET(GetClearColor, SetClearColor, Math::Color ClearColor);
        GET_AND_SET(GetCameraZoom, SetCameraZoom, float CameraZoom);
    };
}
