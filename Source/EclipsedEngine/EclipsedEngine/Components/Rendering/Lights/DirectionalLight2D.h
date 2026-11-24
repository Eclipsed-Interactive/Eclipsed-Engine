#pragma once

#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include <string>
#include "ECS/ComponentManager.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Color.h"

#include "EclipsedEngine.Core.hpp"
#include "Core/GraphicsBuffers/SpriteBuffer.h"
#include "Core/GraphicsBuffers/TransformBuffer.h"

namespace Eclipse
{

    // Prepared for the future.
    class ECL_API DirectionalLight2D : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(DirectionalLight2D);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(DirectionalLight2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        
    private:
        [[SerializeField]]
        Math::Color myColor = { 1.f, 0.96f, 0.88f, 1.f };

        [[SerializeField, Range(0.f, 3.f)]]
        float myIntensity = 0.7f;

        [[SerializeField]]
        float myShadowLength = 1.f;

        [[SerializeField, Range(0.f, 200.f)]]
        int myShadowSteps = 1;
    };
}
