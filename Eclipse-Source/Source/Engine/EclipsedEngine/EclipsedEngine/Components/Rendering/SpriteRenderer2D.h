#pragma once

#include "EclipsedEngine/Components/Rendering/BaseRenderComponent.h"
#include "EclipsedEngine/Components/Component.h"

#include <string>
#include "EntityEngine/ComponentManager.h"

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Color.h"

#include "AssetEngine/Assets/Material.h"
#include "CoreEngine/Macros/defines.h"
#include "CoreEngine/GraphicsBuffers/SpriteBuffer.h"
#include "CoreEngine/GraphicsBuffers/TransformBuffer.h"

namespace Eclipse
{
    class Sprite;
    class Shader;

    class Transform2D;
    
    class ECLIPSED_API SpriteRenderer2D : public BaseRenderComponent
    {
        COMPONENT_BASE_2(SpriteRenderer2D, 20)

    public:
        void OnComponentAdded() override;

        void Render() override;
        void Draw(unsigned aProgramID = 0);

        Texture GetSprite();

        void SetSprite(const std::string& aGuid);
        void SetSprite(const Texture& aSprite);

        void SetMaterial(const std::string& aGuid);
        void SetMaterial(const Material& aMaterial);
        Material* GetMaterial() { return nullptr; }

        void SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax);

        void SetXMirror(bool aMirror);/* { mirroredX = aMirror; }*/
        void SetYMirror(bool aMirror); /*{ mirroredY = aMirror; }*/

        //void DrawInspector() override;

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

    private:
        bool hasSprite = false;
        bool hasMaterial = false;

        bool mirroredX = false;
        bool mirroredY = false;

        MANUAL_REPLICATED_SERIALIZED_FIELD(Texture, sprite, SpriteRenderer2D);
        SERIALIZED_FIELD(Material, material);

        TransformBuffer myTransformBuffer;
        SpriteBuffer mySpriteBuffer;
    };
}
