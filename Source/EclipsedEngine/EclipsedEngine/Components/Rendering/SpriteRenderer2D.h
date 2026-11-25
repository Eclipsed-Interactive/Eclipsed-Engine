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
#include "Core/GraphicsBuffers/MaterialBuffer.h"
#include <AssetSystem/Assets/Types/Material.h>
#include <AssetSystem/Assets/Types/Texture.h>

namespace Eclipse
{
    namespace Graphics
    {
        class IRenderer;
        class IDrawer;
        class IGraphicsDevice;
    }

    class Sprite;
    class Shader;

    class Transform2D;
    
    class ECL_API SpriteRenderer2D : public BaseRenderComponent
    {
        MAKE_REFLECTABLE(SpriteRenderer2D);
        REFLECTION_FLAG(COMPONENT);

        COMPONENT_BASE_2(SpriteRenderer2D, 20)

    public:
        static bool RenderingFrozen;

        void OnComponentAdded() override;

        void Render() override;
        void Draw(unsigned aProgramID = 0);

        void CheckInFrustum();


        void SetSprite(const GUID& aGuid);
        void SetSprite(const Texture& aSprite);
        [[nodiscard]] Texture GetSprite() const { return mySprite; }

        void SetMaterial(const GUID& aGuid);
        void SetMaterial(const Material& aMaterial);
        [[nodiscard]] Material GetMaterial() const { return myMaterial; }

        void SetColor(const Math::Color& aColor);
        [[nodiscard]] Math::Color GetColor() const { return myColor; }

        void SetSpriteRect(const Math::Vector2f& aMin, const Math::Vector2f& aMax);

        void SetXMirror(bool aMirror) { myMirror.x = aMirror; }
        void SetYMirror(bool aMirror) { myMirror.y = aMirror; }


        void OnChangeColor();
        void OnChangeMaterial();
        void OnChangeSprite();
        void OnChangeMirror();
        void OnChangeZIndex();

        Math::Vector2f spriteRectMin = { 0.f, 0.f };
        Math::Vector2f spriteRectMax = { 1.f, 1.f };

    private:
        static Material PixelPickMaterial;

    private:
        bool hasSprite = false;
        bool hasMaterial = false;

        SpriteBuffer mySpriteBuffer;
        MaterialBuffer materialBuffer;
                

        Math::Vector2f Bounds;

        Graphics::IRenderer* renderer;
        Graphics::IDrawer* drawer;

        class EditorBuffer* editorBuffer;
        Graphics::IGraphicsDevice* graphicsDevice;

        bool IsInFrustum = true;

    private:
        [[SerializeField, OnChange(OnChangeSprite)]]
        Texture mySprite;

        [[SerializeField, OnChange(OnChangeMaterial)]]
        Material myMaterial;

        [[SerializeField, OnChange(OnChangeColor)]]
        Math::Color myColor;

        [[SerializeField, OnChange(OnChangeMirror)]]
        Math::Vector2<bool> myMirror = { false, false };
        
        [[SerializeField, OnChange(OnChangeZIndex)]]
        int zIndex = 0;
    };
}
