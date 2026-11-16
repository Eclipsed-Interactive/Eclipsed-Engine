#pragma once

#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include <string>
#include "ECS/ComponentManager.h"

#include "Core/Math/Vector/Vector2.h"
#include "Core/Math/Color.h"

#include "Assets/AssetManager.h"
#include "EclipsedEngine.Core.hpp"
#include "Core/GraphicsBuffers/SpriteBuffer.h"
#include "Core/GraphicsBuffers/TransformBuffer.h"

namespace Eclipse
{
    class ECL_API PointLight2D : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(PointLight2D);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(PointLight2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        void Draw(unsigned aProgramID = 0);

    private:
        Assets::Material myLightMaterial;

        [[SerializeField]]
        Math::Color myColor = { 1.f, 1.f, 1.f, 1.f };

        [[SerializeField]]
        float myRadius = 1.f;

        [[SerializeField]]
        float myIntensity = 1.f;

        [[SerializeField]]
        float myFalloff = 2.f;

        [[SerializeField]]
        float myInnerRadius= 0.5f;
    };
}
