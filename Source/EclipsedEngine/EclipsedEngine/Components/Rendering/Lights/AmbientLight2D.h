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
    class ECL_API AmbientLight2D : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(AmbientLight2D);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(AmbientLight2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        
    private:
        [[SerializeField]]
        Math::Color myColor = { 0.55f, 0.66f, 0.85f, 1.f };

        [[SerializeField, Range(0.f, 3.f)]]
        float myIntensity = 0.5f;
    };
}
