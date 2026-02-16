#pragma once

#include "EclipsedEngine/Components/Component.h"

#include "CoreEngine/Math/Math.h"

namespace Eclipse
{
    class Camera : public Component
    {
        COMPONENT_BASE_2(Camera, 100)

    public:
        void OnDestroy() override;

        void OnComponentAdded() override;
        void OnSceneLoaded() override;
        void Render() override;

        void UpdateCameraTransform();

        static inline class Camera* main;

        bool created;

        static inline bool drawCameraGizmos = false;
    };
}
