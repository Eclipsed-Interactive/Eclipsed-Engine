#pragma once

#include "EclipsedEngine/Components/Component.h"

#include "Core/GraphicsBuffers/CameraBuffer.h"

namespace Eclipse
{
    class Camera : public Component
    {
        COMPONENT_BASE_2(Camera, 100)

    public:
        void OnDestroy() override;

        void OnComponentAdded() override;
        void EditorUpdate() override;
        void OnDrawGizmos() override;

        //void UpdateCameraTransform();

        SERIALIZED_FIELD_STEP_DEFAULT(float, CameraZoom, 0.01f, 1.f);

        //static inline class Camera* main;
        bool created;

        static inline bool drawCameraGizmos = true;

        CameraBuffer myCameraBuffer;

        inline Math::Color GetClearColor() { return ClearColor; }

        Math::Vector2f MinBoundsWorld;
        Math::Vector2f MaxBoundsWorld;

    private:
        Math::Color ClearColor = { 0.2f, 0.3f, 0.4f, 1 };
    };
}
